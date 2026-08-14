-- ==============================================================================
-- Project   : AEL Sovereign — CS50x 2026-2027
-- Module    : week7_movies_pset7
-- File      : movies/4.sql
-- Author    : Ayman Elmasry — AEL Digital Studio
-- ------------------------------------------------------------------------------
-- Description:
--   Query 4: Count how many movies received a rating of exactly 10.0.
--
-- SQL logic:
--   This is an aggregate query over two tables linked by an inner join:
--     1. JOIN   — `movies` and `ratings` are matched on the shared key
--                 (movies.id = ratings.movie_id), so each rating row is
--                 associated with its owning movie.
--     2. WHERE  — keeps only rating rows whose score is exactly 10.0.
--     3. COUNT  — an aggregate function that tallies the number of surviving
--                 rows. COUNT(title) counts non-NULL title values; since a
--                 movie always has a title, this equals the row count.
--   COUNT is why one scalar row (not a list) is returned.
--
-- Returns:
--   A single number: the count of movies rated 10.0.
-- ==============================================================================

SELECT COUNT(title)
FROM movies
JOIN ratings ON movies.id = ratings.movie_id
WHERE rating = 10.0;
