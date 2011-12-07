#include "DesignByContract.hpp"
#include "MathUtils.hpp"
#include "Schedule.hpp"

#include <algorithm>
#include <map>
#include <sstream>

static bool pair_comp(const std::pair<int, int> & pair1, const std::pair<int, int> & pair2) {
   if(pair1.first < pair2.first) {
      return true;
   } else if(pair1.first > pair2.first) {
      return false;
   } else {
      if(pair1.second < pair2.second) {
         return true;
      }
   }
   return false;
}

Schedule::Schedule(size_t people, size_t groups, size_t rounds)
: m_people(people), m_groups(groups), m_rounds(rounds)
{
   ASSERT(this->consistent());
}

void Schedule::push_round(const round_type & round) {
   ASSERT(this->consistent());

   m_schedule.push_back(round);

   ASSERT(this->consistent());
}

void Schedule::pop_round() {
   ASSERT(this->consistent());
   ASSERT(!m_schedule.empty());
   ASSERT(!m_schedule.size() != m_rounds);

   m_schedule.pop_back();

   ASSERT(this->consistent());
}

size_t Schedule::round_size() const {
   return m_schedule.size();
}

bool Schedule::consistent() const {
   if(m_schedule.size() > m_rounds) return false;
   for(schedule_type::const_iterator s_it = m_schedule.begin(); s_it != m_schedule.end(); ++s_it) {
      if(s_it->size() != m_groups) return false;
      for(round_type::const_iterator r_it = s_it->begin(); r_it != s_it->end(); ++r_it) {
         if((*r_it)->size() != m_people) return false;
      }
   }
   return true;
}

bool Schedule::valid_group(const group_type & group, size_t size) const {
   pair_usage_map pair_map(pair_comp);
   for(schedule_type::const_iterator s_it = m_schedule.begin(); s_it != m_schedule.end(); ++s_it) {
      for(round_type::const_iterator r_it = s_it->begin(); r_it != s_it->end(); ++r_it) {
         if(contain_same_pair(group, *(*r_it))) {
            if(size == 1) {
               return false;
            }
            pair_set group_pairs(pair_comp);
            matching_pairs(group, *(*r_it), group_pairs);
            for(pair_set::const_iterator it = group_pairs.begin(); it != group_pairs.end(); ++it) {
               if(size < ++pair_map[*it]) {
                  return false;
               }
            }
         }
      }
   }
   return true;
}

bool Schedule::check(size_t min, size_t max) const {
   // check max
   pair_usage_map pair_map(pair_comp);
   for(schedule_type::const_iterator s_it = m_schedule.begin(); s_it != m_schedule.end(); ++s_it) {
      for(round_type::const_iterator r_it = s_it->begin(); r_it != s_it->end(); ++r_it) {
         pair_set group_pairs(pair_comp);
         pairs(*(*r_it), group_pairs);
         for(pair_set::const_iterator it = group_pairs.begin(); it != group_pairs.end(); ++it) {
            if(max < ++pair_map[*it]) {
               return false;
            }
         }
      }
   }

   // if max passes and minimum is less than 1 this works
   if(min<1) return true;

   static const size_t combinations = MathUtils::comb(m_groups * m_people, 2);
   if(pair_map.size() < combinations) return false;
   if(min == 1) return true;
   for(auto it = pair_map.begin(); it != pair_map.end(); ++it) {
      if(it->second < min) return false;
   }
   return true;
}

bool Schedule::contain_same_pair(const group_type & group1, const group_type & group2) const {
   auto it1 = group1.begin();
   auto it2 = group2.begin();

   size_t count = 0;
   while(it1 != group1.end() && it2 != group2.end()) {
      if(*it1 < *it2) {
         ++it1;
      } else if(*it1 > *it2) {
         ++it2;
      } else {
         count++;
         if(count == 2) return true;
         ++it1;
         ++it2;
      }
   }
   return false;
}

void Schedule::matching_pairs(const group_type & group1, const group_type & group2, pair_set & result) const {
   pair_set pairs1(pair_comp);
   pair_set pairs2(pair_comp);
   pairs(group1, pairs1);
   pairs(group2, pairs2);
   ASSERT(pairs1.size() == pairs2.size());

   std::vector<pair_type> v(pairs1.size());
   auto it = std::set_union(pairs1.begin(), pairs1.end(),
                  pairs2.begin(), pairs2.end(),
                  v.begin(), pair_comp);
   result.insert(v.begin(), it);
}

void Schedule::pairs(const group_type & group, pair_set & pairs) const {
   for(size_t i=0; i<group.size()-1; i++) {
      for(size_t j=i+1; j<group.size(); j++) {
         pairs.insert(std::make_pair(group[i], group[j]));
      }
   }
}

std::string Schedule::to_string(const group_type & group) const {
   group_type::const_iterator it = group.begin();
   std::ostringstream oss;
   oss << "{" << *it;
   ++it;
   for(; it != group.end(); ++it) {
      oss << ", " << *it;
   }
   oss << "}";
   return oss.str();
}

std::string Schedule::to_string(const round_type & round) const {
   round_type::const_iterator it = round.begin();
   std::ostringstream oss;
   oss << to_string(*(*it));
   ++it;
   for(; it != round.end(); ++it) {
      oss << " " << to_string(*(*it));
   }
   oss << std::endl;
   return oss.str();
}

std::string Schedule::to_string(const schedule_type & schedule) const {
   std::ostringstream oss;
   for(schedule_type::const_iterator it = schedule.begin(); it != schedule.end(); ++it) {
      oss << to_string(*it);
   }
   return oss.str();
}

std::ostream & operator<<(std::ostream & os, const Schedule & s) {
   os << s.to_string(s.m_schedule);
   return os;
}
