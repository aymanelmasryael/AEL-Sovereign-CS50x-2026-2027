-- ============================================================
-- AEL Sovereign — CS50x 2026-2027
-- Problem Set 7: Movies — Query 5
-- Author: Ayman Elmasry — AEL Digital Studio
-- ============================================================
-- Write a SQL query to list the titles and release years of all Harry
-- Potter movies, in chronological order.
--
-- Every feature film in the series is titled "Harry Potter and the ...",
-- so the LIKE pattern 'Harry Potter and the%' matches exactly the eight
-- theatrical movies while excluding the companion documentary "Harry
-- Potter: A History of Magic".  ORDER BY year places them in release
-- order.
-- ============================================================

SELECT title, year
FROM movies
WHERE title LIKE 'Harry Potter and the%'
ORDER BY year;
