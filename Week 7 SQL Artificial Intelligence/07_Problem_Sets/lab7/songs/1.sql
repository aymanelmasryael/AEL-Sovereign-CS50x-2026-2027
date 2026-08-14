-- ==============================================================================
-- Project   : AEL Sovereign — CS50x 2026-2027
-- Module    : week7_songs_lab7
-- File      : songs/1.sql
-- Author    : Ayman Elmasry — AEL Digital Studio
-- ------------------------------------------------------------------------------
-- Description:
--   Query 1: List the names of all songs in the database.
--
-- SQL logic:
--   A full-table projection. With no WHERE, JOIN or ORDER BY clauses, every
--   row of the `songs` table is returned, and the SELECT list isolates only
--   the `name` column. This is the simplest possible read of the dataset —
--   a complete enumeration of song titles in storage (id) order.
--
-- Returns:
--   One column, `name`, containing every song title.
-- ==============================================================================

SELECT name
FROM songs;