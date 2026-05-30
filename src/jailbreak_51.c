/**
 * GreenPois0n Absinthe - jailbreak_51.c
 * Copyright (C) 2010 Chronic-Dev Team
 * Copyright (C) 2010 Joshua Hill
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>

#include <plist/plist.h>

#include <libimobiledevice/afc.h>
#include <libimobiledevice/libimobiledevice.h>
#include <libimobiledevice/lockdown.h>

#include "jailbreak_51.h"
#include "bpatch.h"
#include "debug.h"
#include "backup.h"
#include "common.h"
#include "dictionary.h"
#include "endianness.h"
#include "file.h"
#include "idevicebackup2.h"

extern int userpref_get_device_public_key(const char* udid,
		unsigned char** pubkey, unsigned int* pubkey_len);

/* Shared state and helpers defined in jailbreak.c */
extern int connected;
extern int rmdir_recursive_afc(afc_client_t afc, const char *path, int incl);
extern void move_back_files_afc(afc_client_t afc);
extern void get_absinthe_tmpdir(char* pathout);
extern int afc_upload_file(afc_client_t afc, const char* filename, const char* todir);

#define IOS_5_1_AUDIT_INJECT_DIR    ".tmpdir0"
#define IOS_5_1_LOCKDOWN_INJECT_DIR ".tmpdir1"
#define IOS_5_1_OVERRIDES_INJECT_DIR ".tmpdir2"

int jailbreak_51(const char* udid, status_cb_t status_cb,
		device_t* device, lockdown_t* lockdown, const char* product,
		const char* build) {
	char backup_directory[1024];
	get_absinthe_tmpdir(backup_directory);
	strcat(backup_directory, "backup");
	mkdir_with_parents(backup_directory, 0755);

	status_cb("Beginning jailbreak, this may take a while...", 2);

	/********************************************************/
	/* start AFC and move dirs out of the way */
	/********************************************************/
	lockdownd_pair_record_t port = 0;
	if (lockdown_start_service(lockdown, "com.apple.afc", &port) != 0) {
		status_cb("ERROR: Failed to start AFC service", 0);
		lockdown_free(lockdown);
		device_free(device);
		return -1;
	}

	plist_t device_public_key = NULL;
	lockdown_get_value(lockdown, NULL, "DevicePublicKey", &device_public_key);
	if (!device_public_key
			|| (plist_get_node_type(device_public_key) != PLIST_DATA)) {
		status_cb("ERROR: Failed to get key", 0);
		lockdown_free(lockdown);
		device_free(device);
		return -1;
	}

	lockdown_free(lockdown);
	lockdown = NULL;

	afc_client_t afc = NULL;
	afc_client_new(device->client, port, &afc);
	if (!afc) {
		status_cb("ERROR: Could not connect to AFC service", 0);
		device_free(device);
		return -1;
	}

	status_cb(NULL, 4);

	char* testdata = NULL;
	uint64_t testsize = 0;
	plist_get_data_val(device_public_key, &testdata, &testsize);
	const char chk[] = "-----BEGIN RSA PUBLIC KEY-----";
	if (memcmp(testdata, chk, strlen(chk)) != 0) {
		plist_free(device_public_key);
		unsigned char* pkey = NULL;
		unsigned int pkeylen = 0;
		if (userpref_get_device_public_key(udid, &pkey, &pkeylen) != 0) {
			status_cb("ERROR: Unrecoverable error occured...", 0);
			device_free(device);
			return -1;
		}
		uint64_t pklen = pkeylen;
		device_public_key = plist_new_data(pkey, pklen);
	}

	// check if directory exists
	char** list = NULL;
	if (afc_read_directory(afc, "/"AFCTMP, &list) != AFC_E_SUCCESS) {
		// we're good, directory does not exist.
	} else {
		free_dictionary(list);
		status_cb(
				"Looks like you attempted to apply this Jailbreak and it failed. Will try to fix now...",
				0);
		sleep(5);
		goto fix;
	}

	afc_remove_path(afc, "/Books/" IOS_5_1_AUDIT_INJECT_DIR);
	afc_remove_path(afc, "/Books/" IOS_5_1_LOCKDOWN_INJECT_DIR);
	afc_remove_path(afc, "/Books/" IOS_5_1_OVERRIDES_INJECT_DIR);

	if (afc_make_link(afc, AFC_SYMLINK, "../../..",
			"/Books/" IOS_5_1_AUDIT_INJECT_DIR) != AFC_E_SUCCESS) {
		status_cb("ERROR: could not create link!", 0);
		afc_client_free(afc);
		device_free(device);
		return -1;
	}
	if (afc_make_link(afc, AFC_SYMLINK, "../../../root/Library/Lockdown",
			"/Books/" IOS_5_1_LOCKDOWN_INJECT_DIR) != AFC_E_SUCCESS) {
		status_cb("ERROR: could not create link!", 0);
		afc_client_free(afc);
		device_free(device);
		return -1;
	}
	if (afc_make_link(afc, AFC_SYMLINK,
			"../../../db/launchd.db/com.apple.launchd",
			"/Books/" IOS_5_1_OVERRIDES_INJECT_DIR) != AFC_E_SUCCESS) {
		status_cb("ERROR: could not create link!", 0);
		afc_client_free(afc);
		device_free(device);
		return -1;
	}

	status_cb(NULL, 6);
	afc_make_directory(afc, "/"AFCTMP);

	debug("moving dirs aside...\n");
	//afc_rename_path(afc, "/Books", "/"AFCTMP"/Books"); // seems to not break it if we leave it out!!!
	afc_rename_path(afc, "/DCIM", "/"AFCTMP"/DCIM");
	afc_rename_path(afc, "/PhotoData", "/"AFCTMP"/PhotoData");
	afc_rename_path(afc, "/Photos", "/"AFCTMP"/Photos");
	afc_rename_path(afc, "/Recordings", "/"AFCTMP"/Recordings");
	// TODO other paths?

	// make sure directory exists
	afc_make_directory(afc, "/jb-install");

	// remove all files in it
	rmdir_recursive_afc(afc, "/jb-install", 0);

	afc_upload_file(afc, "data/common/Cydia.tar.lzma", "/jb-install");
	afc_upload_file(afc, "data/common/tar", "/jb-install");
	afc_upload_file(afc, "data/common/lzma", "/jb-install");
	afc_upload_file(afc, "data/common/corona2/crazeles", "/jb-install");
	afc_upload_file(afc,
			"data/common/corona2/com.chronic-dev.greenpois0n.rocky-racoon.list",
			"/jb-install");
	afc_upload_file(afc,
			"data/common/corona2/com.chronic-dev.greenpois0n.rocky-racoon.extrainst_",
			"/jb-install");
	afc_upload_file(afc,
			"data/common/corona2/com.chronic-dev.greenpois0n.rocky-racoon.postinst",
			"/jb-install");
	afc_upload_file(afc,
			"data/common/corona2/com.chronic-dev.greenpois0n.rocky-racoon.postrm",
			"/jb-install");
	afc_upload_file(afc,
			"data/common/corona2/com.chronic-dev.greenpois0n.rocky-racoon.preinst",
			"/jb-install");
	afc_upload_file(afc,
			"data/common/corona2/com.chronic-dev.greenpois0n.rocky-racoon.prerm",
			"/jb-install");
	afc_upload_file(afc, "data/common/corona2/remounter", "/jb-install");
	afc_upload_file(afc, "data/common/corona2/dirhelper", "/jb-install");
	afc_upload_file(afc, "data/common/corona2/jb-install", "/jb-install");
	afc_upload_file(afc, "data/common/corona2/jb.plist", "/jb-install");
	afc_upload_file(afc, "data/common/corona2/launchd.conf", "/jb-install");

	afc_client_free(afc);
	afc = NULL;

	status_cb(NULL, 8);

	/********************************************************/
	/* make backup */
	/********************************************************/
	rmdir_recursive(backup_directory);
	__mkdir(backup_directory, 0755);

	status_cb(NULL, 10);

	char *bargv[] = { "idevicebackup2", "backup", backup_directory, NULL };
	idevicebackup2(3, bargv);

	backup_t* backup = backup_open(backup_directory, udid);
	if (!backup) {
		fprintf(stderr, "ERROR: failed to open backup\n");
		return -1;
	}

	status_cb("Preparing jailbreak data...", 20);
	backup_file_t* bf = NULL;

	bf = backup_file_create(NULL );
	if (bf) {
		backup_file_set_domain(bf, "BooksDomain");
		backup_file_set_path(bf,
				IOS_5_1_LOCKDOWN_INJECT_DIR "/device_public_key.pem");
		backup_file_set_target(bf, "/usr/sbin/racoon");
		backup_file_set_mode(bf, 0120644);
		backup_file_set_inode(bf, 54327);
		backup_file_set_uid(bf, 0);
		backup_file_set_gid(bf, 0);
		unsigned int tm = (unsigned int) (time(NULL ));
		backup_file_set_time1(bf, tm);
		backup_file_set_time2(bf, tm);
		backup_file_set_time3(bf, tm);
		backup_file_set_flag(bf, 0);

		if (backup_update_file(backup, bf) < 0) {
			fprintf(stderr, "ERROR: could not add file to backup\n");
		}
		backup_file_free(bf);
	}

	backup_write_mbdb(backup);
	backup_free(backup);

	/********************************************************/
	/* restore backup WITHOUT rebooting */
	/********************************************************/
	status_cb("Sending initial jailbreak data. This may also take a while...",
			30);
	char* nargv[] = { "idevicebackup2", "restore", "--system", "--settings",
			backup_directory, NULL };
	idevicebackup2(5, nargv);

	/********************************************************/
	/* crash lockdownd */
	/********************************************************/
	idevice_connection_t conn = NULL;
	if (idevice_connect(device->client, 0xf27e, &conn) != IDEVICE_E_SUCCESS) {
		status_cb("ERROR: could not connect to lockdownd", 0);
		device_free(device);
		return -1;
	}

	plist_t crashme = plist_new_dict();
	plist_dict_insert_item(crashme, "Request", plist_new_string("Pair"));
	plist_dict_insert_item(crashme, "PairRecord", plist_new_bool(0));

	char* cxml = NULL;
	uint32_t clen = 0;

	plist_to_xml(crashme, &cxml, &clen);
	plist_free(crashme);
	crashme = NULL;

	uint32_t bytes = 0;
	uint32_t nlen = htobe32(clen);
	idevice_connection_send(conn, (const char*) &nlen, 4, &bytes);
	idevice_connection_send(conn, cxml, clen, &bytes);
	free(cxml);

	int failed = 0;
	bytes = 0;
	clen = 0;
	cxml = NULL;
	idevice_connection_receive_timeout(conn, (char*) &clen, 4, &bytes, 1500);
	nlen = be32toh(clen);
	if (nlen > 0) {
		cxml = malloc(nlen);
		idevice_connection_receive_timeout(conn, cxml, nlen, &bytes, 5000);
		free(cxml);
		if (bytes > 0) {
			failed = 1;
		}
	}
	idevice_disconnect(conn);
	if (failed) {
		status_cb("ERROR: could not stroke lockdownd", 0);
		device_free(device);
		return -1;
	}

	/********************************************************/
	/* get racoon from lockdownd */
	/********************************************************/
	lockdownd_client_t lckd = NULL;
	lockdownd_client_new(device->client, &lckd, NULL );

	plist_t racoon_plist = NULL;
	lockdownd_get_value(lckd, NULL, "DevicePublicKey", &racoon_plist);
	if (!racoon_plist || (plist_get_node_type(racoon_plist) != PLIST_DATA)) {
		status_cb("ERROR: Failed to get racoon", 0);
		device_free(device);
		return -1;
	}

	char* racoon_data = NULL;
	uint64_t racoon_size = 0;
	plist_get_data_val(racoon_plist, &racoon_data, &racoon_size);

	// patch the racoon file we got with entitlement exploit
	unsigned int got = 0;
	debug("Writing racoon to filesystem\n");
	char racoon_path[1024];
	get_absinthe_tmpdir(racoon_path);
	mkdir_with_parents(racoon_path, 0755);
	strcat(racoon_path, "racoon");

	got = file_write(racoon_path, racoon_data, (unsigned int) racoon_size);
	if (got == -1) {
		error("Unable to write data to filesystem\n");
	}
	if (racoon_data) {
		free(racoon_data);
	}

	debug("patching racoon\n");
	bpatch_t* patch = bpatch_open("data/common/rocky-racoon/racoon.bdiff");
	debug("patch open\n");
	if (patch != NULL ) {
		if (bpatch_apply(patch, racoon_path) != 0) {
			fprintf(stderr, "ERROR: Failed to patch target\n");
		}
		bpatch_free(patch);
	} else {
		fprintf(stderr, "ERROR: failed to open racoon.bdiff\n");
		return -1;
	}

	device_free(device);
	device = NULL;

	backup = backup_open(backup_directory, udid);
	if (!backup) {
		fprintf(stderr, "ERROR: failed to open backup\n");
		return -1;
	}

	// restore device_public_key.pem
	bf = backup_get_file(backup, "BooksDomain",
			IOS_5_1_LOCKDOWN_INJECT_DIR "/device_public_key.pem");
	if (bf) {
		char* key_data = NULL;
		uint64_t key_size = 0;
		plist_get_data_val(device_public_key, &key_data, &key_size);
		backup_file_assign_file_data(bf, (unsigned char*) key_data, key_size,
				0);
		backup_file_set_target(bf, NULL );
		backup_file_set_mode(bf, 0100644);
		backup_file_set_inode(bf, 54327);
		backup_file_set_uid(bf, 0);
		backup_file_set_gid(bf, 0);
		unsigned int tm = (unsigned int) (time(NULL ));
		backup_file_set_time1(bf, tm);
		backup_file_set_time2(bf, tm);
		backup_file_set_time3(bf, tm);
		backup_file_set_length(bf, key_size);
		backup_file_set_flag(bf, 4);
		backup_file_update_hash(bf);

		if (backup_update_file(backup, bf) < 0) {
			fprintf(stderr, "ERROR: could not add file to backup\n");
		}
		backup_file_free(bf);
		if (key_data) {
			free(key_data);
		}
	}

	backup_add_file(backup, "data/common/rocky-racoon/overrides.plist", 0644,
			"BooksDomain", IOS_5_1_OVERRIDES_INJECT_DIR "/overrides.plist");

	// create audit directory
	bf = backup_file_create(NULL );
	backup_file_set_domain(bf, "BooksDomain");
	backup_file_set_path(bf, IOS_5_1_AUDIT_INJECT_DIR "/audit");
	backup_file_set_mode(bf, 040755);
	backup_file_set_inode(bf, 54320);
	backup_file_set_uid(bf, 0);
	backup_file_set_gid(bf, 0);
	unsigned int tm = (unsigned int) (time(NULL ));
	backup_file_set_time1(bf, tm);
	backup_file_set_time2(bf, tm);
	backup_file_set_time3(bf, tm);
	backup_file_set_length(bf, 0);
	backup_file_set_flag(bf, 0);
	if (backup_update_file(backup, bf) < 0) {
		fprintf(stderr, "ERROR: could not add file to backup\n");
	}
	backup_file_free(bf);

	// create rocky-racoon directory
	bf = backup_file_create(NULL );
	backup_file_set_domain(bf, "BooksDomain");
	backup_file_set_path(bf, IOS_5_1_AUDIT_INJECT_DIR "/audit/rocky-racoon");
	backup_file_set_mode(bf, 040755);
	backup_file_set_inode(bf, 54321);
	backup_file_set_uid(bf, 501);
	backup_file_set_gid(bf, 501);
	tm = (unsigned int) (time(NULL ));
	backup_file_set_time1(bf, tm);
	backup_file_set_time2(bf, tm);
	backup_file_set_time3(bf, tm);
	backup_file_set_length(bf, 0);
	backup_file_set_flag(bf, 0);
	if (backup_update_file(backup, bf) < 0) {
		fprintf(stderr, "ERROR: could not add file to backup\n");
	}
	backup_file_free(bf);

	int i;
	char rocky_file[1024];
	char rocky_target[256];

	// add 00-ff files
	for (i = 0; i < 256; i++) {
		sprintf(rocky_file, "data/%s/%s/rocky-racoon/%02x", build, product, i);
		sprintf(rocky_target,
				IOS_5_1_AUDIT_INJECT_DIR "/audit/rocky-racoon/%02x", i);
		backup_add_file(backup, rocky_file, 0644, "BooksDomain", rocky_target);
	}

	// add boot.conf
	sprintf(rocky_file,
			"data/%s/%s/rocky-racoon/%s", build, product, "boot.conf");
	sprintf(rocky_target,
			IOS_5_1_AUDIT_INJECT_DIR "/audit/rocky-racoon/%s", "boot.conf");
	backup_add_file(backup, rocky_file, 0644, "BooksDomain", rocky_target);

	// add install.conf
	sprintf(rocky_file,
			"data/%s/%s/rocky-racoon/%s", build, product, "install.conf");
	sprintf(rocky_target,
			IOS_5_1_AUDIT_INJECT_DIR "/audit/rocky-racoon/%s", "install.conf");
	backup_add_file(backup, rocky_file, 0644, "BooksDomain", rocky_target);

	// add racoon
	sprintf(rocky_target,
			IOS_5_1_AUDIT_INJECT_DIR "/audit/rocky-racoon/%s", "racoon");
	backup_add_file(backup, racoon_path, 0755, "BooksDomain", rocky_target);

	backup_write_mbdb(backup);
	backup_free(backup);

	plist_free(device_public_key);

	// remove racoon
	remove(racoon_path);

	/********************************************************/
	/* restore backup */
	/********************************************************/
	status_cb(
			"Sending final jailbreak data. Your device will appear to be restoring a backup, this may also take a while...",
			35);
	char* rargv[] = { "idevicebackup2", "restore", "--system", "--settings",
			"--reboot", backup_directory, NULL };
	idevicebackup2(6, rargv);

	rmdir_recursive(backup_directory);

	status_cb(
			"Waiting for reboot — not done yet, don't unplug your device yet!",
			70);

	/********************************************************/
	/* wait for device reboot */
	/********************************************************/

	// wait for disconnect
	while (connected) {
		sleep(2);
	} debug("Device %s disconnected\n", udid);
	status_cb(NULL, 75);

	// wait for device to connect
	while (!connected) {
		sleep(2);
	} debug("Device %s detected. Connecting...\n", udid);
	status_cb("Waiting for process to complete...", 80);
	sleep(2);

	/********************************************************/
	/* connect and wait for completion */
	/********************************************************/
	device = device_create(udid);
	if (!device) {
		status_cb("ERROR: Could not connect to device. Aborting.\n", 0);
		return -1;
	}

	int retries = 30;
	int done = 0;
	while (!done && (retries-- > 0)) {
		lockdown = lockdown_open(device);
		if (!lockdown) {
			device_free(device);
			status_cb("ERROR: Could not connect to lockdownd. Aborting.\n", 0);
			return -1;
		}
		port = 0;
		if (lockdown_start_service2(lockdown, "com.apple.afc2", &port, 0)
				== 0) {
			done = 1;
			break;
		}
		lockdown_free(lockdown);
		lockdown = NULL;
		sleep(5);
	}
	if (lockdown) {
		port = 0;
		if (lockdown_start_service(lockdown, "com.apple.afc", &port) == 0) {
			afc_client_new(device->client, port, &afc);
			if (afc) {
				afc_remove_path(afc, "/Books/" IOS_5_1_AUDIT_INJECT_DIR);
				afc_remove_path(afc, "/Books/" IOS_5_1_LOCKDOWN_INJECT_DIR);
				afc_remove_path(afc, "/Books/" IOS_5_1_OVERRIDES_INJECT_DIR);
				afc_client_free(afc);
				afc = NULL;
			}
		}

		lockdown_free(lockdown);
		lockdown = NULL;
	}

	if (done) {
		status_cb("Done, enjoy!", 100);
	} else {
		status_cb(
				"Hmm... something seems to have gone wrong... trying to recover",
				80);
		sleep(3);
		goto fix;
	}

	goto leave;

	fix:
	/********************************************************/
	/* move back any remaining dirs via AFC */
	/********************************************************/
	status_cb("Recovering files...", 80);
	if (!afc) {
		lockdown = lockdown_open(device);
		port = 0;
		if (lockdown_start_service(lockdown, "com.apple.afc", &port) != 0) {
			status_cb("ERROR: Could not start AFC service. Aborting.", 0);
			lockdown_free(lockdown);
			goto leave;
		}
		lockdown_free(lockdown);

		afc_client_new(device->client, port, &afc);
		if (!afc) {
			status_cb("ERROR: Could not connect to AFC. Aborting.\n", 0);
			goto leave;
		}
	}

	status_cb(NULL, 90);

	move_back_files_afc(afc);

	status_cb(NULL, 95);
	afc_remove_path(afc, "/"AFCTMP);
	if (afc_read_directory(afc, "/"AFCTMP, &list) == AFC_E_SUCCESS) {
		fprintf(stderr,
				"WARNING: the folder /"AFCTMP" is still present in the user's Media folder. You have to check yourself for any leftovers and move them back if required.\n");
	}

	afc_remove_path(afc, "/Books/" IOS_5_1_AUDIT_INJECT_DIR);
	afc_remove_path(afc, "/Books/" IOS_5_1_LOCKDOWN_INJECT_DIR);
	afc_remove_path(afc, "/Books/" IOS_5_1_OVERRIDES_INJECT_DIR);

	rmdir_recursive(backup_directory);

	status_cb(
			"Recovery completed. If you want to retry jailbreaking, unplug your device and plug it back in.",
			100);

	leave: if (afc) {
		afc_client_free(afc);
		afc = NULL;
	}

	device_free(device);
	device = NULL;

	return 0;
}
