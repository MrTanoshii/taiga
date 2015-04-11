/*
** Taiga
** Copyright (C) 2010-2014, Eren Okka
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

#ifndef TAIGA_TRACK_RECOGNITION_H
#define TAIGA_TRACK_RECOGNITION_H

#include <map>
#include <set>
#include <string>
#include <vector>

#include "base/string.h"

namespace anime {
class Episode;
class Item;
}

namespace track {
namespace recognition {

typedef std::map<int, double> scores_t;
typedef std::vector<std::pair<int, double>> sorted_scores_t;

struct MatchOptions {
  bool allow_sequels = false;
  bool check_airing_date = false;
  bool check_anime_type = false;
  bool check_episode_number = false;
};

class Engine {
public:
  bool Parse(std::wstring title, anime::Episode& episode) const;
  int Identify(anime::Episode& episode, bool give_score, const MatchOptions& match_options);
  bool Search(const std::wstring& title, std::vector<int>& anime_ids);

  void InitializeTitles();
  void UpdateTitles(const anime::Item& anime_item);

  sorted_scores_t GetScores() const;

  bool IsValidAnimeType(const anime::Episode& episode) const;
  bool IsValidFileExtension(const anime::Episode& episode) const;
  bool IsValidFileExtension(const std::wstring& extension) const;

  bool ReadRelations();
  bool SearchEpisodeRedirection(int id, const std::pair<int, int>& range, int& destination_id, std::pair<int, int>& destination_range) const;

private:
  bool ValidateOptions(anime::Episode& episode, int anime_id, const MatchOptions& match_options, bool redirect) const;
  bool ValidateOptions(anime::Episode& episode, const anime::Item& anime_item, const MatchOptions& match_options, bool redirect) const;
  bool ValidateEpisodeNumber(anime::Episode& episode, const anime::Item& anime_item, const MatchOptions& match_options, bool redirect) const;

  int LookUpTitle(std::wstring title, std::set<int>& anime_ids) const;
  bool GetTitleFromPath(anime::Episode& episode);

  int ScoreTitle(anime::Episode& episode, const std::set<int>& anime_ids, const MatchOptions& match_options);
  int ScoreTitle(const std::wstring& str, const anime::Episode& episode, const scores_t& trigram_results);

  void Normalize(std::wstring& title, int type, bool normalized_before) const;
  void NormalizeUnicode(std::wstring& str) const;
  void ErasePunctuation(std::wstring& str, int type) const;
  void EraseUnnecessary(std::wstring& str) const;
  void ConvertOrdinalNumbers(std::wstring& str) const;
  void ConvertRomanNumbers(std::wstring& str) const;
  void ConvertSeasonNumbers(std::wstring& str) const;
  void Transliterate(std::wstring& str) const;

  struct Titles {
    typedef std::map<std::wstring, std::set<int>> container_t;
    container_t alternative;
    container_t main;
    container_t user;
  } normal_titles_, titles_;

  struct ScoreStore {
    std::vector<std::wstring> normal_titles;
    std::vector<trigram_container_t> trigrams;
  };
  std::map<int, ScoreStore> db_;
  sorted_scores_t scores_;
};

}  // namespace recognition
}  // namespace track

extern track::recognition::Engine Meow;

#endif  // TAIGA_TRACK_RECOGNITION_H