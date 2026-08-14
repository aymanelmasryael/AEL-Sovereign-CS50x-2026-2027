-- ============================================================
-- AEL Sovereign — CS50x 2026-2027
-- Problem Set 7: Movies — Query 7
-- Author: Ayman Elmasry — AEL Digital Studio
-- ============================================================
-- Write a SQL query to list all movies released in 2010 and their
-- ratings, ordered from highest to lowest rating.  For movies sharing
-- the same rating, list them alphabetically by title.
--
-- The join links each movie to its rating via the shared movie id, and
-- the WHERE clause isolates the 2010 releases.  ORDER BY sorts by rating
-- descending first, then by title ascending, exactly as the problem asks.
-- ============================================================

SELECT title, rating
FROM movies
JOIN ratings ON ratings.movie_id = movies.id
WHERE movies.year = 2010
ORDER BY rating DESC, title;