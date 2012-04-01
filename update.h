/*
** Taiga, a lightweight client for MyAnimeList
** Copyright (C) 2010-2012, Eren Okka
** 
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
** 
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
** 
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef UPDATE_H
#define UPDATE_H

#include "std.h"
#include "http.h"

// =============================================================================

class VersionInfo {
public:
  wstring major, minor, revision, build, url;
};

class UpdateFile {
public:
  bool download;
  wstring path, checksum;
};

class UpdateHelper {
public:
  UpdateHelper();
  virtual ~UpdateHelper() {}

  virtual void OnCheck() {}
  virtual void OnCRCCheck(const wstring& path, wstring& crc) {}
  virtual void OnDone() {}
  virtual void OnProgress(int file_index) {}
  virtual bool OnRestartApp() { return true; }
  virtual void OnRunActions() {}

  bool Check(const wstring& address, CApp& app, DWORD client_mode);
  bool DownloadNextFile(DWORD client_mode);
  bool ParseData(wstring data, DWORD client_mode);
  bool RestartApplication(const wstring& updatehelper_exe, 
    const wstring& current_exe, const wstring& new_exe);

public:
  vector<wstring> actions;
  class HttpClient client;
  vector<UpdateFile> files;
  bool restart_app, update_available;
  VersionInfo version_info;

private:
  CApp* app_;
};

#endif // UPDATE_H