-- ============================================================
-- AEL Sovereign — CS50x 2026-2027
-- Lab 7: Songs — Query 5
-- Author: Ayman Elmasry — AEL Digital Studio
-- ============================================================
-- Write a SQL query that returns the average energy of all the songs.
--
-- AVG is an aggregate function that folds the energy column of every
-- row in the songs table into a single arithmetic mean, yielding one
-- cell as the result.
-- ============================================================

SELECT AVG(energy)
FROM songs;
