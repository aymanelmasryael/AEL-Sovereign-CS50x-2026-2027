-- ==============================================================================
-- Project   : AEL Sovereign — CS50x 2026-2027
-- Module    : week7_movies_pset7
-- File      : movies/10.sql
-- Author    : Ayman Elmasry — AEL Digital Studio
-- ------------------------------------------------------------------------------
-- Description:
--   Query 10: List the distinct names of all people whose directed movies
--   earned a rating of at least 9.0.
--
-- SQL logic:
--   A three-table join resolving directors → movies → ratings:
--     1. `people`    JOIN `directors` ON people.id = directors.person_id
--              — pairs every person with each film they directed.
--     2. `directors` JOIN `ratings`   ON directors.movie_id = ratings.movie_id
--              — attaches the rating of each directed film. Note the join
--                goes directors.movie_id directly to ratings (movies is
--                skipped because ratings already carries movie_id).
--     3. WHERE rating >= 9.0 — keeps only highly rated directed films.
--     4. DISTINCT            — ensures a director with several high-rated
--                films is listed exactly once.
--
-- Returns:
--   Unique director names whose filmography includes a film rated >= 9.0.
-- ==============================================================================

SELECT DISTINCT name
FROM people
JOIN directors ON people.id = directors.person_id
JOIN ratings ON directors.movie_id = ratings.movie_id
WHERE rating >= 9.0;