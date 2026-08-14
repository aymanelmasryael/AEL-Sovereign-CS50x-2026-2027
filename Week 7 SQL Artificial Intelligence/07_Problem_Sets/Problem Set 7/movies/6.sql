-- ==============================================================================
-- Project   : AEL Sovereign — CS50x 2026-2027
-- Module    : week7_movies_pset7
-- File      : movies/6.sql
-- Author    : Ayman Elmasry — AEL Digital Studio
-- ------------------------------------------------------------------------------
-- Description:
--   Query 6: Determine the average rating of all movies released in 2012.
--
-- SQL logic:
--   A two-table aggregate:
--     1. JOIN   — `ratings` joined to `movies` on the shared key
--                 (ratings.movie_id = movies.id), linking each rating to the
--                 movie it belongs to.
--     2. WHERE  — filters the joined result to only movies from 2012.
--     3. AVG    — computes the arithmetic mean of the `rating` column over
--                 the surviving rows.
--   AVG collapses many rows into one scalar summary value. Note that AVG
--   ignores NULL rating values; every rated 2012 movie thus contributes once.
--
-- Returns:
--   A single number: the mean rating across all 2012 releases.
-- ==============================================================================

SELECT AVG(rating)
FROM ratings
JOIN movies ON ratings.movie_id = movies.id
WHERE year = 2012;
