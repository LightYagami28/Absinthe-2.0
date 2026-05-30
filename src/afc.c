/**
 * GreenPois0n Absinthe - afc.c
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
#include <libimobiledevice/afc.h>

#include "afc.h"
#include "debug.h"
#include "lockdown.h"

afc_t* afc_create() {
	afc_t* afc = (afc_t*) malloc(sizeof(afc_t));
	if(afc != NULL) {
		memset(afc, '\0', sizeof(afc));
	}
	return afc;
}

afc_t* afc_connect(device_t* device) {
	int err = 0;
	uint16_t port = 0;
	afc_t* afc = NULL;
	lockdown_t* lockdown = NULL;

	lockdown = lockdown_open(device);
	if(lockdown == NULL) {
		error("Unable to open connection to lockdownd\n");
		return NULL;
	}

	err = lockdown_start_service(lockdown, "com.apple.afc", &port);
	if(err < 0) {
		error("Unable to start AFC service\n");
		return NULL;
	}
	lockdown_close(lockdown);

	afc = afc_open(device, port);
	if(afc == NULL) {
		error("Unable to open connection to AFC service\n");
		return NULL;
	}

	return afc;
}

afc_t* afc_open(device_t* device, uint16_t port) {
	afc_error_t err = AFC_E_SUCCESS;
	afc_t* afc = afc_create();
	if(afc != NULL) {
		err = afc_client_new(device->client, port, &(afc->client));
		if(err != AFC_E_SUCCESS) {
			error("Unable to create new MobileBackup2 client\n");
			afc_free(afc);
			return NULL;
		}
		afc->device = device;
		afc->port = port;
	}
	return afc;
}

int afc_close(afc_t* afc) {
	//TODO: Implement Me
	return -1;
}

void afc_free(afc_t* afc) {
	if (afc) {
		free(afc);
	}
}

static void afc_free_dictionary(char **dictionary) //ghetto i know, not sure where/how to put a global function for this
{
	int i = 0;

	if (!dictionary)
		return;

	for (i = 0; dictionary[i]; i++) {
		free(dictionary[i]);
	}
	free(dictionary);
}

void apparition_afc_get_file_contents(afc_t* afc, const char *filename,
		char **data, uint64_t *size) {
	if (!afc || !data || !size) {
		return;
	}

	char **fileinfo = NULL;
	uint32_t fsize = 0;

	afc_get_file_info(afc->client, filename, &fileinfo);
	if (!fileinfo) {
		return;
	}
	int i;
	for (i = 0; fileinfo[i]; i += 2) {
		if (!strcmp(fileinfo[i], "st_size")) {
			fsize = atol(fileinfo[i + 1]);
			break;
		}
	}
	afc_free_dictionary(fileinfo);

	if (fsize == 0) {
		return;
	}

	uint64_t f = 0;
	afc_file_open(afc->client, filename, AFC_FOPEN_RDONLY, &f);
	if (!f) {
		return;
	}
	char *buf = (char*) malloc((uint32_t) fsize);
	uint32_t done = 0;
	while (done < fsize) {
		uint32_t bread = 0;
		afc_file_read(afc->client, f, buf + done, 65536, &bread);
		if (bread > 0) {

		} else {
			break;
		}
		done += bread;
	}
	if (done == fsize) {
		*size = fsize;
		*data = buf;
	} else {
		free(buf);
	}
	afc_file_close(afc->client, f);
}

int afc_send_file(afc_t* afc, const char* local, const char* remote) {
	if (!afc || !local || !remote) return -1;

	// Read local file contents
	FILE* f = fopen(local, "rb");
	if (!f) {
		error("afc_send_file: cannot open %s\n", local);
		return -1;
	}
	fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
	fseek(f, 0, SEEK_SET);
	if (fsize <= 0) { fclose(f); return -1; }

	char* contents = malloc((size_t)fsize);
	if (!contents) { fclose(f); return -1; }

	if (fread(contents, 1, (size_t)fsize, f) != (size_t)fsize) {
		fclose(f); free(contents); return -1;
	}
	fclose(f);

	// Open remote file via AFC
	uint64_t my_file = 0;
	afc_file_open(afc->client, remote, AFC_FOPEN_WR, &my_file);
	if (!my_file) { free(contents); return -1; }

	uint32_t bytes = 0;
	afc_error_t err = afc_file_write(afc->client, my_file, contents, (uint32_t)fsize, &bytes);
	free(contents);
	afc_file_close(afc->client, my_file);

	if (err != AFC_E_SUCCESS || bytes != (uint32_t)fsize) {
		error("afc_send_file: sent %u of %ld bytes\n", bytes, fsize);
		return -1;
	}

	printf("afc all done.\n");
	return 0;
}
