#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include "Schedule.hpp"

#include <cstddef>
#include <functional>
#include <set>
#include <vector>

class Scheduler {
public:
   typedef std::vector< Schedule > solution_set;
   typedef Schedule::group_type group_type;
   typedef Schedule::round_type round_type;

   enum eCount { ONE, ALL };

private:
   typedef std::function<bool(const group_type &, const group_type &)> group_less;
   typedef std::set< group_type, group_less > group_set;

public:
   Scheduler(size_t people, size_t groups);

   // Solve for a schedule with given constraints, returns number of solutions found
   size_t solve(eCount count = ALL);

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
   size_t solve_next_round(eCount count, Schedule & schedule, const group_set & group_comb);

   /**
    * Find the next group in round
    *
    * @param[in] schedule   current schedule being solved for
    * @param[in] round      current round being solved for
    * @param[in] group_set  set of groups still valid in schedule
    * @param[in] cur        A pointer in group_set where to continue trying groups
    * @return               number of solutions with schedule plus round
    */
   size_t solve_next_group(eCount count, Schedule & schedule, round_type & round, const group_set & group_comb, group_set::const_iterator cur);

   bool valid_group(const group_type & group, const round_type & round) const;

   void remove_invalid_groups(const Schedule & schedule, group_set & group_comb) const;

   /// Calculate all the group combinations possible
   void group_combinations(group_set & group_comb) const;
   void group_combinations(group_set & group_comb, group_type & group, size_t cur, size_t depth) const;

   size_t m_people;
   size_t m_groups;
   size_t m_rounds;
   solution_set m_solutions;
};

#endif // SCHEDULER_HPP
