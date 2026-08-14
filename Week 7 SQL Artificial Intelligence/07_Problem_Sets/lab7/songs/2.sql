-- ============================================================
-- AEL Sovereign — CS50x 2026-2027
-- Lab 7: Songs — Query 2
-- Author: Ayman Elmasry — AEL Digital Studio
-- ============================================================
-- Write a SQL query to list the names of all songs in increasing order
-- of tempo.
--
-- ORDER BY tempo sorts the single-column result from the slowest track
-- to the fastest; ascending is SQL's default direction, so no keyword
-- is required.
-- ============================================================

SELECT name
FROM songs
ORDER BY tempo;
