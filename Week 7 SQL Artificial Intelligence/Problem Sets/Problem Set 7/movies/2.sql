-- ============================================================
-- AEL Sovereign — CS50x 2026-2027
-- Problem Set 7: Movies — Query 2
-- Author: Ayman Elmasry — AEL Digital Studio
-- ============================================================
-- Write a SQL query to determine the birth year of Emma Stone.
--
-- The people table stores every person who appears in the database,
-- including their name and birth year.  Filtering on the name column
-- identifies Emma Stone's single record; the birth column holds the
-- answer.  The problem guarantees that only one Emma Stone exists.
-- ============================================================

SELECT birth
FROM people
WHERE name = 'Emma Stone';
