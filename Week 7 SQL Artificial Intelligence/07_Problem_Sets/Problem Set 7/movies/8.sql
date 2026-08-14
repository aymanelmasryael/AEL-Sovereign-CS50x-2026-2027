-- ==============================================================================
-- Project   : AEL Sovereign — CS50x 2026-2027
-- Module    : week7_movies_pset7
-- File      : movies/8.sql
-- Author    : Ayman Elmasry — AEL Digital Studio
-- ------------------------------------------------------------------------------
-- Description:
--   Query 8: Find the names of all people who starred in the movie
--   "Toy Story".
--
-- SQL logic:
--   A three-table star-relationship join, following the people ⟨starred-in⟩
--   movies relational chain:
--     1. `people`  JOIN `stars`  ON people.id  = stars.person_id
--              — pairs each person with every starring credit they hold.
--     2. `stars`   JOIN `movies` ON stars.movie_id = movies.id
--              — pairs each credit with its film.
--     3. WHERE title = "Toy Story"
--              — keeps only pairing rows whose film is "Toy Story".
--   Traversing people→stars→movies this way converts the many-to-many
--   relationship (actors ↔ films) into concrete (person, movie) pairs.
--
-- Returns:
--   One column, `name`, for every cast member of "Toy Story".
-- ==============================================================================

SELECT name
FROM people
JOIN stars ON people.id = stars.person_id
JOIN movies ON stars.movie_id = movies.id
WHERE title = "Toy Story";