#if !defined(KORE_CONSOLE)

#include "filewriter.h"

#include <kinc/error.h>
#include <kinc/log.h>
#include <kinc/system.h>

#include <stdio.h>
#include <string.h>

#if defined(KORE_WINDOWS)
#define NOMINMAX
#include <Windows.h>
#endif

#if defined(KORE_PS4) || defined(KORE_SWITCH)
#define MOUNT_SAVES
bool mountSaveData(bool);
void unmountSaveData();
#endif

bool kinc_file_writer_open(kinc_file_writer_t *writer, const char *filepath) {
	writer->file = NULL;
	writer->mounted = false;
#ifdef MOUNT_SAVES
	if (!mountSaveData(true)) {
		return false;
	}
	writer->mounted = true;
#endif
	char path[1001];
	strcpy(path, kinc_internal_save_path());
	strcat(path, filepath);

#ifdef KORE_WINDOWS
	wchar_t wpath[MAX_PATH];
	MultiByteToWideChar(CP_UTF8, 0, path, -1, wpath, MAX_PATH);
	writer->file = _wfopen(wpath, L"wb");
#else
	writer->file = fopen(path, "wb");
#endif
	if (writer->file == NULL) {
		kinc_log(KINC_LOG_LEVEL_WARNING, "Could not open file %s.", filepath);
		return false;
	}
	return true;
}

void kinc_file_writer_close(kinc_file_writer_t *writer) {
	if (writer->file != NULL) {
		fclose((FILE *)writer->file);
		writer->file = NULL;
	}
#ifdef MOUNT_SAVES
	if (writer->mounted) {
		writer->mounted = false;
		unmountSaveData();
	}
#endif
}

void kinc_file_writer_write(kinc_file_writer_t *writer, void *data, int size) {
	fwrite(data, 1, size, (FILE *)writer->file);
}

#endif
