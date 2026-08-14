-- ==============================================================================
-- Project   : AEL Sovereign — CS50x 2026-2027
-- Module    : week7_movies_pset7
-- File      : movies/9.sql
-- Author    : Ayman Elmasry — AEL Digital Studio
-- ------------------------------------------------------------------------------
-- Description:
--   Query 9: List the distinct names of all people who starred in a movie
--   released in 2004, ordered by the person's birth year.
--
-- SQL logic:
--   A three-table join followed by deduplication and a cross-table sort:
--     1. `people` JOIN `stars` ON people.id = stars.person_id
--              — attaches each starring credit to its actor.
--     2. `stars`  JOIN `movies` ON stars.movie_id = movies.id
--              — attaches each credit to its film.
--     3. WHERE year = 2004
--              — keeps only credits for 2004 releases.
--     4. DISTINCT — removes duplicate person rows: an actor starring in
--              several 2004 films would otherwise appear once per film.
--     5. ORDER BY birth — sorts the deduplicated names by each person's
--              birth year (ascending), which is legal even though `birth`
--              is not in the SELECT list because it belongs to `people`.
--
-- Returns:
--   Unique actor names for 2004 films, ordered by birth year ascending.
-- ==============================================================================

SELECT DISTINCT name
FROM people
JOIN stars ON people.id = stars.person_id
JOIN movies ON stars.movie_id = movies.id
WHERE year = 2004
ORDER BY birth;