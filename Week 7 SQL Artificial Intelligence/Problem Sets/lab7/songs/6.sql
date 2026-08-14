-- ============================================================
-- AEL Sovereign — CS50x 2026-2027
-- Lab 7: Songs — Query 6
-- Author: Ayman Elmasry — AEL Digital Studio
-- ============================================================
-- Write a SQL query that lists the names of songs that are by Post
-- Malone.
--
-- The songs table stores only an artist_id, so the artist's id must be
-- resolved first.  A nested subquery looks up Post Malone's id by name,
-- and the outer query selects every song whose artist_id matches that
-- value.  Nesting makes the query correct even if his id ever changed.
-- ============================================================

SELECT name
FROM songs
WHERE artist_id = (
    SELECT id
    FROM artists
    WHERE name = 'Post Malone'
);
