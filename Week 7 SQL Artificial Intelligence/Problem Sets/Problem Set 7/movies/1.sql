-- ============================================================
-- AEL Sovereign — CS50x 2026-2027
-- Problem Set 7: Movies — Query 1
-- Author: Ayman Elmasry — AEL Digital Studio
-- ============================================================
-- Write a SQL query to list the titles of all movies released in 2008.
--
-- The movies table stores one row per film: a unique id, the film's
-- title, and the year it was released.  Filtering on the year column
-- with an equality comparison yields every 2008 release.
-- ============================================================

SELECT title
FROM movies
WHERE year = 2008;
