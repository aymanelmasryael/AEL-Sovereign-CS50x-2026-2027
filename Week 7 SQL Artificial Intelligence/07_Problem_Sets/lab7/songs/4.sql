-- ============================================================
-- AEL Sovereign — CS50x 2026-2027
-- Lab 7: Songs — Query 4
-- Author: Ayman Elmasry — AEL Digital Studio
-- ============================================================
-- Write a SQL query that lists the names of any songs that have
-- danceability, energy, and valence greater than 0.75.
--
-- Each of the three audio features is a real number between 0 and 1 in
-- the songs table.  Chaining three strict-greater-than comparisons with
-- AND keeps only the rows where every one of the features clears the
-- 0.75 threshold.
-- ============================================================

SELECT name
FROM songs
WHERE danceability > 0.75
  AND energy > 0.75
  AND valence > 0.75;
