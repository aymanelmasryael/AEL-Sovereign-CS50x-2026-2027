-- ==============================================================================
-- Project   : AEL Sovereign — CS50x 2026-2027
-- Module    : week7_movies_pset7
-- File      : movies/11.sql
-- Author    : Ayman Elmasry — AEL Digital Studio
-- ------------------------------------------------------------------------------
-- Description:
--   Query 11: List the titles of the five highest-rated movies featuring
--   Chadwick Boseman, from best to worst.
--
-- SQL logic:
--   A four-table join (movies → stars → people → ratings) plus ranking:
--     1. `movies` JOIN `stars`  ON movies.id = stars.movie_id
--              — associates each film with its cast credits.
--     2. `stars`  JOIN `people` ON stars.person_id = people.id
--              — resolves each credit to the actor's identity.
--     3. `movies` JOIN `ratings` ON movies.id = ratings.movie_id
--              — attaches the rating score to each movie.
--     4. WHERE name = "Chadwick Boseman"
--              — narrows to films Boseman appears in.
--     5. ORDER BY rating DESC — ranks those films by score, highest first.
--     6. LIMIT 5 — returns only the top five rows (the best-rated films).
--
-- Returns:
--   A single `title` column with at most five of Boseman's films, ordered by
--   descending rating.
-- ==============================================================================

SELECT title
FROM movies
JOIN stars ON movies.id = stars.movie_id
JOIN people ON stars.person_id = people.id
JOIN ratings ON movies.id = ratings.movie_id
WHERE name = "Chadwick Boseman"
ORDER BY rating DESC
LIMIT 5;