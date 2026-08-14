-- ==============================================================================
-- Project   : AEL Sovereign — CS50x 2026-2027
-- Module    : week7_songs_lab7
-- File      : songs/5.sql
-- Author    : Ayman Elmasry — AEL Digital Studio
-- ------------------------------------------------------------------------------
-- Description:
--   Query 5: Compute the average energy across all songs in the database.
--
-- SQL logic:
--   A whole-table aggregate. With no WHERE clause, AVG(energy) is evaluated
--   over every row of `songs`. The aggregate function collapses the entire
--   energy column into one scalar mean value. AVG ignores NULL entries, so
--   only tracks with a recorded energy score influence the mean.
--
-- Returns:
--   A single number: the mean energy of all songs.
-- ==============================================================================

SELECT AVG(energy)
FROM songs;