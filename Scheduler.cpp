#include "Scheduler.hpp"
#include "DesignByContract.hpp"

#include <iostream>
#include <vector>
#include <algorithm>

Scheduler::Scheduler(size_t people, size_t groups)
   : m_people(people), m_groups(groups), m_min(1), m_max(1)
{
   // for now assume only everyone with everyone else exactly once
   m_rounds = ((m_people * m_groups) - 1) / (m_people - 1);
   ASSERT((m_rounds * (m_people - 1) + 1) == (m_people * m_groups)); // Fix for cases like this to find best solution
#ifdef DEBUG
   std::cout << "Required Rounds: " << m_rounds << std::endl;
#endif
}

Scheduler::Scheduler(size_t people, size_t groups, size_t rounds)
   : m_people(people), m_groups(groups), m_rounds(rounds), m_min(1), m_max(1)
{
   ASSERT(m_rounds <= ((m_people * m_groups) - 1) / (m_people - 1));
}

Scheduler::Scheduler(size_t people, size_t groups, size_t rounds, size_t min, size_t max)
   : m_people(people), m_groups(groups), m_rounds(rounds), m_min(min), m_max(max)
{
#ifdef DEBUG
   size_t req_rounds = (((m_people * m_groups) - 1) / (m_people - 1));
   ASSERT(req_rounds * m_min <= m_rounds);
   ASSERT(m_rounds <= req_rounds * m_max);
#endif
}

size_t Scheduler::solve(eCount count) {
   m_solutions.clear();

   // A set of groups ordered by the group members
   group_set group_comb([](const group_ptr & u, const group_ptr & v) -> bool {
      ASSERT(u->size() == v->size());
      for(size_t i=0; i<u->size(); i++) {
         if((*u)[i] < (*v)[i]) {
            return true;
         } else if((*u)[i] > (*v)[i]) {
            return false;
         }
      }
      return false;
   });

   group_combinations(group_comb);

   Schedule schedule(m_people, m_groups, m_rounds);
   round_type round;
   int cur = 0;
   for(size_t i=0; i<m_groups; i++) {
      group_type group;
      for(size_t j=0; j<m_people; j++) {
         group.push_back(cur++);
      }
      ASSERT(group.size() == m_people);
      round.push_back(group);

      // Remove from group_comb, it is no longer valid, ASSUMING ONLY ONE ONE PAIRING
      group_set::const_iterator it = group_comb.find(std::make_shared<group_type>(group));
      ASSERT(it != group_comb.end());
      group_comb.erase(it);
   }
   ASSERT(round.size() == m_groups);

   schedule.push_round(round);
   return solve_next_round(count, schedule, group_comb, group_comb.begin());
}

size_t Scheduler::solve_next_round(eCount count, Schedule & schedule, const group_set & group_comb, const group_set::const_iterator & first_group_it) {
   if(m_rounds == schedule.round_size() && schedule.check(m_min, m_max)) {
      // we have a solution
      m_solutions.push_back(schedule);
      return 1;
   }

   group_set group_comb_copy(first_group_it, group_comb.end(), group_comb.value_comp());
   remove_invalid_groups(schedule, group_comb_copy);
   return solve_first_group_next_round(count, schedule, group_comb_copy);
}

size_t Scheduler::solve_first_group_next_round(eCount count, Schedule & schedule, const group_set & group_comb) {
   size_t solutions = 0;
   round_type round;
   group_set::const_iterator it = group_comb.begin();
   // group_comb's groups are in lexicographic order, therefore all the groups containing 0 are at the begining
   while(it != group_comb.end() && (*(*it))[0] == 0) {
      round.push_back(*(*it));
      ++it;
      solutions += solve_next_group(count, schedule, round, group_comb, it, it);
      round.pop_back();
   }
   return solutions;
}

size_t Scheduler::solve_next_group(eCount count, Schedule & schedule, round_type & round, const group_set & group_comb, group_set::const_iterator cur, const group_set::const_iterator & first_group_it) {
   size_t solutions = 0;
#ifdef UNKNOWN_OPTIMIZATION
   if(std::distance(it, group_comb.end()) < static_cast<int>(m_groups)) {
      return solutions;
   }
#endif
   for(; cur != group_comb.end(); ++cur) {
      if(!valid_group(*(*cur), round)) continue;
      round.push_back(*(*cur));
      if(round.size() == m_groups) {
         schedule.push_round(round);
         solutions += solve_next_round(count, schedule, group_comb, first_group_it);
         schedule.pop_round();
      } else {
         ++cur;
         solutions += solve_next_group(count, schedule, round, group_comb, cur, first_group_it);
         --cur;
      }
      if(solutions > 0 && count == ONE) break;
      round.pop_back();
   }
   return solutions;
}

bool Scheduler::valid_group(const group_type & group, const round_type & round) const {
   for(round_type::const_iterator r_it = round.begin(); r_it != round.end(); ++r_it) {
      for(group_type::const_iterator g_it = group.begin(); g_it != group.end(); ++g_it) {
         if(std::find(r_it->begin(), r_it->end(), *g_it) != r_it->end()) {
            return false;
         }
      }
   }
   return true;
}

void Scheduler::remove_invalid_groups(const Schedule & schedule, group_set & group_comb) const {
   group_set::const_iterator it = group_comb.begin();
   while(it != group_comb.end()) {
      group_set::const_iterator cur = it++;
      if(!schedule.valid_group(*(*cur))) {
         group_comb.erase(cur);
      }
   }
}

void Scheduler::group_combinations(group_set & group_comb) const {
   group_type group(m_people);
   group_combinations(group_comb, group, 0, 1);
}

void Scheduler::group_combinations(group_set & group_comb, group_type & group, size_t cur, size_t depth) const {
   size_t max = ((m_groups - 1) * m_people) + depth;
   for(size_t i=cur; i<max; i++) {
      group.at(depth-1) = i;
      if(depth == m_people) {
         group_comb.insert(std::make_shared<group_type>(group));
      } else {
         group_combinations(group_comb, group, i+1, depth+1);
      }
   }
}
