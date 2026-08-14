-- ============================================================
-- AEL Sovereign — CS50x 2026-2027
-- Lab 7: Songs — Query 8
-- Author: Ayman Elmasry — AEL Digital Studio
-- ============================================================
-- Write a SQL query that lists the names of the songs that feature other
-- artists.
--
-- Collaborations are flagged inside the track title with the text
-- "feat.".  The LIKE pattern '%feat.%' matches any name containing that
-- literal substring at any position, so every featured collaboration is
-- captured in the result.
-- ============================================================

SELECT name
FROM songs
WHERE name LIKE '%feat.%';
