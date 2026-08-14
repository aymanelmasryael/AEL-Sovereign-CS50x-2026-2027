-- ============================================================
-- AEL Sovereign — CS50x 2026-2027
-- Problem Set 7: Movies — Query 9
-- Author: Ayman Elmasry — AEL Digital Studio
-- ============================================================
-- Write a SQL query to list the id and name of every person who starred
-- in a movie released in 2004, ordered by birth year.
--
-- The id (not just the name) is selected because two different people in
-- the database can share the same name; the id uniquely identifies each
-- actor.  DISTINCT ensures a person appears only once even if they
-- starred in several 2004 films.  The chain people -> stars -> movies
-- pairs every actor with the films they starred in, and the WHERE clause
-- isolates the 2004 releases.  ORDER BY people.birth sorts the results
-- from youngest-... oldest-scan (birth year ascending).
-- ============================================================

SELECT DISTINCT people.id, people.name
FROM people
JOIN stars ON stars.person_id = people.id
JOIN movies ON movies.id = stars.movie_id
WHERE movies.year = 2004
ORDER BY people.birth;