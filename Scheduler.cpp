#include "Scheduler.hpp"
#include "DesignByContract.hpp"

#include <iostream>
#include <vector>
#include <algorithm>

Scheduler::Scheduler(size_t people, size_t groups) : m_people(people), m_groups(groups) {
   // for now assume only everyone with everyone else exactly once
   m_rounds = ((m_people * m_groups) - 1) / (m_people - 1);
   ASSERT((m_rounds * (m_people - 1) + 1) == (m_people * m_groups)); // Fix for cases like this to find best solution
#ifdef DEBUG
   std::cout << "Required Rounds: " << m_rounds << std::endl;
#endif
}

size_t Scheduler::solve() const {
   // A set of groups ordered by the group members
   group_set group_comb([](const group_type & u, const group_type & v) -> bool {
      ASSERT(u.size() == v.size());
      for(size_t i=0; i<u.size(); i++) {
         if(u[i] < v[i]) {
            return true;
         } else if(u[i] > v[i]) {
            return false;
         }
      }
      FAIL_WITH_MESSAGE("Not expected u==v");
      return false;
   });

   group_combinations(group_comb);
   for(group_set::const_iterator it = group_comb.begin(); it != group_comb.end(); ++it) {
      print(*it); std::cout << std::endl;
   }

   schedule_type schedule;
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
      group_set::iterator it = group_comb.find(group);
      ASSERT(it != group_comb.end());
      group_comb.erase(it);
   }
   ASSERT(round.size() == m_groups);

   schedule.push_back(round);
   return solve(schedule, group_comb);
}

size_t Scheduler::solve(schedule_type & schedule, group_set & group_comb) const {
   if(m_rounds == schedule.size()) {
      // we have a solution
      std::cout << "Solution: " << std::endl;
      print(schedule);
      std::cout << std::endl;
      return 1;
   }

   round_type round;
   return solve(schedule, round, group_comb, group_comb.begin());
}

size_t Scheduler::solve(schedule_type & schedule, round_type & round, group_set & group_comb, group_set::const_iterator cur) const {
   size_t solutions = 0;
   for(; cur != group_comb.end(); ++cur) {
      if(!valid_group(*cur, round)) continue;
      if(!valid_group(*cur, schedule)) continue;
      round.push_back(*cur);
      if(round.size() == m_groups) {
         schedule.push_back(round);
         solutions += solve(schedule, group_comb);
         schedule.pop_back();
      } else {
         cur++;
         solutions += solve(schedule, round, group_comb, cur);
         cur--;
      }
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

bool Scheduler::valid_group(const group_type & group, const schedule_type & schedule) const {
   for(schedule_type::const_iterator s_it = schedule.begin(); s_it != schedule.end(); ++s_it) {
      if(s_it->size() != m_groups) continue;
      for(round_type::const_iterator r_it = s_it->begin(); r_it != s_it->end(); ++r_it) {
         if(contain_same_pair(group, *r_it)) {
            return false;
         }
      }
   }
   return true;
}

bool Scheduler::contain_same_pair(const group_type & group1, const group_type & group2) const {
   pair_set group1_pairs;
   pairs(group1, group1_pairs);
   pair_set group2_pairs;
   pairs(group2, group2_pairs);

   for(pair_set::const_iterator it = group1_pairs.begin(); it != group1_pairs.end(); ++it) {
      if(group2_pairs.find(*it) != group2_pairs.end()) {
         return true;
      }
   }
   return false;
}

void Scheduler::pairs(const group_type & group, pair_set & pairs) const {
   for(size_t i=0; i<group.size()-1; i++) {
      for(size_t j=i+1; j<group.size(); j++) {
         pairs.insert(std::make_pair(group[i], group[j]));
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
         group_comb.insert(group);
      } else {
         group_combinations(group_comb, group, i+1, depth+1);
      }
   }
}

void Scheduler::print(const group_type & group) const {
   group_type::const_iterator it = group.begin();
   std::cout << "{" << *it;
   ++it;
   for(; it != group.end(); ++it) {
      std::cout << ", " << *it;
   }
   std::cout << "}";
}

void Scheduler::print(const round_type & round) const {
   round_type::const_iterator it = round.begin();
   print(*it);
   ++it;
   for(; it != round.end(); ++it) {
      std::cout << " ";
      print(*it);
   }
   std::cout << std::endl;
}

void Scheduler::print(const schedule_type & schedule) const {
   for(schedule_type::const_iterator it = schedule.begin(); it != schedule.end(); ++it) {
      print(*it);
   }
}
