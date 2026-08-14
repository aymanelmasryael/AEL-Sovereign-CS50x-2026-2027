-- ============================================================
-- AEL Sovereign — CS50x 2026-2027
-- Problem Set 7: Movies — Query 4
-- Author: Ayman Elmasry — AEL Digital Studio
-- ============================================================
-- Write a SQL query to determine the number of movies with an IMDb
-- rating of 10.0.
--
-- The ratings table holds one row per movie (movie_id) together with a
-- rating.  COUNT(*) tallies the rows that match the filter, so the
-- result is a single cell holding the number of perfect 10.0 films.
-- A JOIN is unnecessary because only the ratings table is inspected.
-- ============================================================

SELECT COUNT(*)
FROM ratings
WHERE rating = 10.0;
