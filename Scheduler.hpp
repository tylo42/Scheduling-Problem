#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include <cstddef>
#include <functional>
#include <set>
#include <vector>

class Scheduler {
private:
   typedef std::vector<int> group_type;
   typedef std::vector< group_type > round_type;
   typedef std::vector< round_type > schedule_type;
   typedef std::function<bool(const group_type &, const group_type &)> group_less;
   typedef std::set< group_type, group_less > group_set;
   typedef std::set<std::pair<int, int> > pair_set;

public:
   typedef std::vector< schedule_type > solution_set;

public:
   Scheduler(size_t people, size_t groups);

   // Solve for a schedule with given constraints, returns number of solutions found
   size_t solve();

   solution_set solutions() const { return m_solutions; }

private:
   // Helper functions

   /**
    * Returns one if schedule is a solution, else start solving for the next round
    *
    * @param[in] schedule   current schedule being solved for
    * @param[in] group_set  set of groups still valid in schedule
    * @return               number of solutions with schedule
    */
   size_t solve(schedule_type & schedule, group_set & group_set);

   /**
    * Find the next group in round
    *
    * @param[in] schedule   current schedule being solved for
    * @param[in] round      current round being solved for
    * @param[in] group_set  set of groups still valid in schedule
    * @param[in] cur        A pointer in group_set where to continue trying groups
    * @return               number of solutions with schedule plus round
    */
   size_t solve(schedule_type & schedule, round_type & round, group_set & group_set, group_set::const_iterator cur);

   bool valid_group(const group_type & group, const round_type & round) const;
   bool valid_group(const group_type & group, const schedule_type & schedule) const;

   bool contain_same_pair(const group_type & group1, const group_type & group2) const;

   void pairs(const group_type & group, pair_set & pairs) const;

   /// Calculate all the group combinations possible
   void group_combinations(group_set & group_comb) const;
   void group_combinations(group_set & group_comb, group_type & group, size_t cur, size_t depth) const;

   /// print the contents of the container
   void print(const group_type & group) const;
   void print(const round_type & round) const;
   void print(const schedule_type & schedule) const;

   size_t m_people;
   size_t m_groups;
   size_t m_rounds;
   solution_set m_solutions;
};

#endif
