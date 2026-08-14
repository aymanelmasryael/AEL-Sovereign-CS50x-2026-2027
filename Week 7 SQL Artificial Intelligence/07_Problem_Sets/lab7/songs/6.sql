-- ==============================================================================
-- Project   : AEL Sovereign — CS50x 2026-2027
-- Module    : week7_songs_lab7
-- File      : songs/6.sql
-- Author    : Ayman Elmasry — AEL Digital Studio
-- ------------------------------------------------------------------------------
-- Description:
--   Query 6: List the names of all songs performed by Post Malone.
--
-- SQL logic:
--   An outer query parameterised by a scalar subquery:
--     1. Inner subquery  — SELECT id FROM artists WHERE name = 'Post Malone'
--              resolves the artist's primary key. It is scalar (one row, one
--              column) because artist names are unique, so it can feed a
--              single-value equality comparison.
--     2. Outer query     — SELECT name FROM songs WHERE artist_id = (…)
--              selects every song whose artist_id FK equals the resolved id.
--   The INNER/OUTER structure decouples the lookup of the artist key from the
--   song listing, so no explicit JOIN is needed — the subquery acts as the
--   join between `songs.artist_id` and `artists.id`.
--
-- Returns:
--   One column, `name`, listing each of Post Malone's songs.
-- ==============================================================================

SELECT name
FROM songs
WHERE artist_id = (SELECT id FROM artists WHERE name = 'Post Malone');