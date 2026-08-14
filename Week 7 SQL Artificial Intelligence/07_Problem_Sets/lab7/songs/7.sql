-- ============================================================
-- AEL Sovereign — CS50x 2026-2027
-- Lab 7: Songs — Query 7
-- Author: Ayman Elmasry — AEL Digital Studio
-- ============================================================
-- Write a SQL query that returns the average energy of songs that are
-- by Drake.
--
-- The artist's name lives in the artists table while the song's energy
-- lives in the songs table, so the two tables are joined on the shared
-- artist id.  The WHERE clause restricts the joined rows to Drake's
-- tracks and AVG computes their mean energy in a single cell.
-- ============================================================

SELECT AVG(energy)
FROM songs
JOIN artists ON artists.id = songs.artist_id
WHERE artists.name = 'Drake';
