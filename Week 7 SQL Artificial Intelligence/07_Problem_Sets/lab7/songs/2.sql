-- ==============================================================================
-- Project   : AEL Sovereign — CS50x 2026-2027
-- Module    : week7_songs_lab7
-- File      : songs/2.sql
-- Author    : Ayman Elmasry — AEL Digital Studio
-- ------------------------------------------------------------------------------
-- Description:
--   Query 2: List the names of all songs in increasing order of tempo.
--
-- SQL logic:
--   A full-table projection with a single sort key. ORDER BY tempo ASC sorts
--   the result set by the tempo value from slowest to fastest; ASC is the
--   default ordering for numeric columns, so it is written explicitly for
--   clarity. No filtering or joining is required.
--
-- Returns:
--   One column, `name`, with every song title sorted by ascending tempo.
-- ==============================================================================

SELECT name
FROM songs
ORDER BY tempo ASC;