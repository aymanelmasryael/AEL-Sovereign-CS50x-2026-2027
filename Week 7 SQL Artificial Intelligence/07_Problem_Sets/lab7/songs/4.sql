-- ==============================================================================
-- Project   : AEL Sovereign — CS50x 2026-2027
-- Module    : week7_songs_lab7
-- File      : songs/4.sql
-- Author    : Ayman Elmasry — AEL Digital Studio
-- ------------------------------------------------------------------------------
-- Description:
--   Query 4: List the names of songs with high danceability, energy AND
--   valence — all three audio-feature scores above 0.75.
--
-- SQL logic:
--   A conjunctive (AND) range filter over three independent audio attributes.
--   Each condition is a strict greater-than comparison on a REAL column, and
--   the AND operator means a row survives only if ALL three hold:
--       danceability > 0.75  AND  energy > 0.75  AND  valence > 0.75.
--   This expresses the intersection of three feature thresholds, returning
--   only tracks that are simultaneously danceable, energetic and upbeat.
--   NULL feature values would be excluded automatically (NULL comparisons
--   evaluate to unknown → row dropped).
--
-- Returns:
--   One column, `name`, for every song meeting all three thresholds.
-- ==============================================================================

SELECT name
FROM songs
WHERE danceability > 0.75 AND energy > 0.75 AND valence > 0.75;