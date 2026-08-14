-- ============================================================
-- AEL Sovereign — CS50x 2026-2027
-- Problem Set 7: Movies — Query 12
-- Author: Ayman Elmasry — AEL Digital Studio
-- ============================================================
-- Write a SQL query to list the titles of all movies in which both
-- Bradley Cooper and Jennifer Lawrence starred.
--
-- A movie qualifies only when it appears in the stars table for BOTH
-- actors, so the movies table is joined to two independent copies of the
-- stars/people pair.  The first copy tests for Jennifer Lawrence's
-- credit and the second for Bradley Cooper's; requiring both conditions
-- keeps exactly the movies where the two performed together.  DISTINCT
-- guards against duplicate titles.
-- ============================================================

SELECT DISTINCT movies.title
FROM movies
JOIN stars AS lawrence ON lawrence.movie_id = movies.id
JOIN people AS lawrence_people ON lawrence_people.id = lawrence.person_id
JOIN stars AS cooper ON cooper.movie_id = movies.id
JOIN people AS cooper_people ON cooper_people.id = cooper.person_id
WHERE lawrence_people.name = 'Jennifer Lawrence'
  AND cooper_people.name = 'Bradley Cooper';