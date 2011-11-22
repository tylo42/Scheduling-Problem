#include "Schedule.hpp"

#include <sstream>

Schedule::Schedule(size_t people, size_t groups, size_t rounds)
: m_people(people), m_groups(groups), m_rounds(rounds)
{}

void Schedule::push_round(const round_type & round) {
   m_schedule.push_back(round);
}

void Schedule::pop_round() {
   m_schedule.pop_back();
}

size_t Schedule::round_size() const {
   return m_schedule.size();
}

bool Schedule::valid_group(const group_type & group) const {
   for(schedule_type::const_iterator s_it = m_schedule.begin(); s_it != m_schedule.end(); ++s_it) {
      if(s_it->size() != m_groups) continue;
      for(round_type::const_iterator r_it = s_it->begin(); r_it != s_it->end(); ++r_it) {
         if(contain_same_pair(group, *r_it)) {
            return false;
         }
      }
   }
   return true;
}

bool Schedule::contain_same_pair(const group_type & group1, const group_type & group2) const {
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
   oss << to_string(*it);
   ++it;
   for(; it != round.end(); ++it) {
      oss << " " << to_string(*it);
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
