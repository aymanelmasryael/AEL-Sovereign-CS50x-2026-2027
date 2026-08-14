-- ==============================================================================
-- Project   : AEL Sovereign — CS50x 2026-2027
-- Module    : week7_movies_pset7
-- File      : movies/7.sql
-- Author    : Ayman Elmasry — AEL Digital Studio
-- ------------------------------------------------------------------------------
-- Description:
--   Query 7: List the titles and ratings of all 2010 movies, ordered by
--   rating (highest first); tie ratings are broken alphabetically by title.
--
-- SQL logic:
--   A join + filter + multi-key sort:
--     1. JOIN   — `movies` joined to `ratings` on movies.id = ratings.movie_id
--                 so each title pairs with its rating score.
--     2. WHERE  — restricts to releases from 2010.
--     3. ORDER  — a two-key sort: PRIMARY rating DESC (largest first), and
--                 SECONDARY title ASC to impose a deterministic, stable order
--                 among movies sharing the identical rating.
--   Multi-column ORDER BY gives a lexicographic ranking (reduce to rating,
--   then title), which is exactly the requested presentation.
--
-- Returns:
--   Columns `title` and `rating` for each 2010 movie, ordered as described.
-- ==============================================================================

SELECT title, rating
FROM movies
JOIN ratings ON movies.id = ratings.movie_id
WHERE year = 2010
ORDER BY rating DESC, title;
