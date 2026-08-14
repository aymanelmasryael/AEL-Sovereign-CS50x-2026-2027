-- ============================================================
-- AEL Sovereign — CS50x 2026-2027
-- Problem Set 7: Movies — Query 6
-- Author: Ayman Elmasry — AEL Digital Studio
-- ============================================================
-- Write a SQL query to determine the average IMDb rating of all movies
-- released in 2012.
--
-- The rating for a film lives in the ratings table, keyed by movie_id,
-- while the release year lives in the movies table.  Joining the two
-- tables on their shared movie id pairs every film with its rating; the
-- WHERE clause restricts the join to 2012 releases and AVG aggregates
-- those ratings into a single value.
-- ============================================================

SELECT AVG(rating)
FROM ratings
JOIN movies ON movies.id = ratings.movie_id
WHERE movies.year = 2012;
