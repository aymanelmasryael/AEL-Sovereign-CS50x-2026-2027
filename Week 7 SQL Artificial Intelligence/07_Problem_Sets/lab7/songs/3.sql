-- ==============================================================================
-- Project   : AEL Sovereign — CS50x 2026-2027
-- Module    : week7_songs_lab7
-- File      : songs/3.sql
-- Author    : Ayman Elmasry — AEL Digital Studio
-- ------------------------------------------------------------------------------
-- Description:
--   Query 3: List the names of the five longest songs (by duration).
--
-- SQL logic:
--   Projection + descending sort + row cap. ORDER BY duration_ms DESC ranks
--   songs from the longest duration to the shortest, and LIMIT 5 discards all
--   but the first five rows — i.e. the five most time-consuming tracks.
--   Tie-breaking among equal durations is unspecified, which is acceptable
--   because LIMIT only bounds the size of the result set.
--
-- Returns:
--   One column, `name`, containing the five longest songs.
-- ==============================================================================

SELECT name
FROM songs
ORDER BY duration_ms DESC
LIMIT 5;