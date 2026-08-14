-- ==============================================================================
-- Project   : AEL Sovereign — CS50x 2026-2027
-- Module    : week7_songs_lab7
-- File      : songs/8.sql
-- Author    : Ayman Elmasry — AEL Digital Studio
-- ------------------------------------------------------------------------------
-- Description:
--   Query 8: List the names of all songs that feature another artist
--   (their title contains "feat.").
--
-- SQL logic:
--   A LIKE pattern match against the title text. The `%` wildcards on BOTH
--   sides — '%feat.%' — match any title that CONTAINS the literal substring
--   "feat." anywhere (prefix, middle or suffix). In SQLite, LIKE is
--   case-insensitive for ASCII by default, so "Feat.", "FEAT." and "feat."
--   all satisfy the pattern, catching collaborator credits regardless of
--   capitalisation.
--
-- Returns:
--   One column, `name`, listing every song whose title advertises a guest
--   artist via the "feat." tag.
-- ==============================================================================

SELECT name
FROM songs
WHERE name LIKE '%feat.%';