-- ============================================================
-- AEL Sovereign — CS50x 2026-2027
-- Problem Set 7: Movies — Query 3
-- Author: Ayman Elmasry — AEL Digital Studio
-- ============================================================
-- Write a SQL query to list the titles of all movies released on or
-- after 2018, in alphabetical order.
--
-- A single-table scan of movies is sufficient.  The WHERE clause keeps
-- every film whose year is 2018 or later, and ORDER BY sorts the result
-- titles alphabetically (ascending order is SQL's default).
-- ============================================================

SELECT title
FROM movies
WHERE year >= 2018
ORDER BY title;
