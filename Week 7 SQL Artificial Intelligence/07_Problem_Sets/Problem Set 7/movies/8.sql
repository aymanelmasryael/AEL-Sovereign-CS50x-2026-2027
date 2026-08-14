-- ============================================================
-- AEL Sovereign — CS50x 2026-2027
-- Problem Set 7: Movies — Query 8
-- Author: Ayman Elmasry — AEL Digital Studio
-- ============================================================
-- Write a SQL query to list the names of all people who starred in the
-- movie "Toy Story".
--
-- Three tables are involved: movies holds the film, people holds the
-- actors, and stars is the many-to-many bridge that records which person
-- starred in which movie.  The join chain navigates person -> stars ->
-- movie, and the WHERE clause pins the chain to the Toy Story film.
-- ============================================================

SELECT people.name
FROM people
JOIN stars ON stars.person_id = people.id
JOIN movies ON movies.id = stars.movie_id
WHERE movies.title = 'Toy Story';