-- ==============================================================================
-- Project   : AEL Sovereign — CS50x 2026-2027
-- Module    : week7_songs_lab7
-- File      : songs/7.sql
-- Author    : Ayman Elmasry — AEL Digital Studio
-- ------------------------------------------------------------------------------
-- Description:
--   Query 7: Compute the average energy of all songs by Drake.
--
-- SQL logic:
--   An aggregate with a correlated scalar subquery filter:
--     1. Inner subquery  — SELECT id FROM artists WHERE name = 'Drake'
--              resolves Drake's artist id.
--     2. WHERE artist_id = (…) — restricts the aggregation to Drake's songs.
--     3. AVG(energy) — averages the energy score over only those rows.
--   Aggregates are evaluated AFTER the WHERE filter, so the mean reflects
--   Drake's catalogue alone. AVG ignores NULL energy values.
--
-- Returns:
--   A single number: the mean energy of Drake's songs.
-- ==============================================================================

SELECT AVG(energy)
FROM songs
WHERE artist_id = (SELECT id FROM artists WHERE name = 'Drake');