/*
 * This file is part of BBP Pairings, a Swiss-system chess tournament engine
 * Copyright (C) 2016  Jeremy Bierema
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License, version 3.0, as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef SWISSSYSTEMSCOMMON_H
#define SWISSSYSTEMSCOMMON_H

#include <deque>
#include <functional>
#include <list>
#include <ostream>
#include <stdexcept>
#include <string>

#include <tournament/tournament.h>

namespace swisssystems
{
  enum SwissSystem
  {
    BURSTEIN, NONE
  };

  /**
   * An exception indicating that no pairing satisfies the requirements imposed
   * by the system.
   */
  struct NoValidPairingException : public std::runtime_error
  {
    NoValidPairingException() : std::runtime_error("") { }
    NoValidPairingException(const std::string &message)
      : std::runtime_error(message) { }
  };

  /**
   * An exception indicating that the chosen Swiss system does not support all
   * of the selected options, for example, nonstandard point systems.
   */
  struct UnapplicableFeatureException : public std::runtime_error
  {
    UnapplicableFeatureException(const std::string &message)
      : std::runtime_error(message) { }
  };

  /**
   * An object representing the assignment of two people to player each other,
   * along with the assignment of colors.
   */
  struct Pairing
  {
    tournament::player_index white;
    tournament::player_index black;

    Pairing(
        const tournament::player_index white_,
        const tournament::player_index black_)
      : white(white_), black(black_) { }
    Pairing(
        const tournament::player_index player0,
        const tournament::player_index player1,
        const tournament::Color player0color)
      : white(player0color == tournament::COLOR_WHITE ? player0 : player1),
        black(player0color == tournament::COLOR_WHITE ? player1 : player0) { }
  };

  /**
   * An object representing info about a Swiss system, including a matching
   * computer and information about acceleration rules.
   */
  struct Info
  {
    virtual std::list<Pairing> computeMatching(
      tournament::Tournament &&,
      std::ostream *
    ) const = 0;
    /**
     * Assign accelerations for the next round, assuming a default acceleration
     * system is specified for this Swiss system. Otherwise, throw an
     * UnsupportedFeatureException.
     */
    virtual void updateAccelerations(tournament::Tournament &) const
    {
      throw UnapplicableFeatureException(
        "The selected Swiss system does not have a default acceleration system."
      );
    }

  protected:
    constexpr Info() noexcept { }
  };

  /**
   * Retrieve the Info object associated with the specified SwissSystem.
   */
  const Info &getInfo(SwissSystem);

  /**
   * Check whether two players can play each other under the normal (pre-last
   * round) restrictions imposed on all Swiss systems.
   */
  inline bool colorPreferencesAreCompatible(
    tournament::Color preference0,
    tournament::Color preference1)
  {
    return
      preference0 != preference1
        || preference0 == tournament::COLOR_NONE
        || preference1 == tournament::COLOR_NONE;
  }

  void findFirstColorDifference(
    const tournament::Player &,
    const tournament::Player &,
    tournament::Color &,
    tournament::Color &);

  void sortResults(std::list<Pairing> &, const tournament::Tournament &);

  void printChecklist(
    std::ostream &,
    const std::deque<std::string> &,
    const std::function<std::deque<std::string>(const tournament::Player &)> &,
    const tournament::Tournament &,
    const std::list<const tournament::Player *> &);
}

#endif