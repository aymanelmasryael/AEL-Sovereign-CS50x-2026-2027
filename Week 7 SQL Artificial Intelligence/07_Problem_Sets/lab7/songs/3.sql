-- ============================================================
-- AEL Sovereign — CS50x 2026-2027
-- Lab 7: Songs — Query 3
-- Author: Ayman Elmasry — AEL Digital Studio
-- ============================================================
-- Write a SQL query to list the names of the top 5 longest songs, in
-- descending order of length.
--
-- Each song's length in milliseconds is stored in the duration_ms
-- column.  ORDER BY duration_ms DESC ranks the tracks from longest to
-- shortest, and LIMIT 5 retains only the five longest songs.
-- ============================================================

SELECT name
FROM songs
ORDER BY duration_ms DESC
LIMIT 5;
