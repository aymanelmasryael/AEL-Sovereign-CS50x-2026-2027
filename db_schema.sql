-- AEL Sovereign Database Schema & Cloud Persistence Structure
-- Designed by Ayman Elmasry for Supabase / Firebase / PostgreSQL / SQLite Core
-- Version: v3.0 Production Architecture

-- 1. Sovereign Users Table
CREATE TABLE IF NOT EXISTS sovereign_users (
    user_id UUID PRIMARY KEY DEFAULT gen_random_uuid(),
    username VARCHAR(255) UNIQUE NOT NULL,
    email VARCHAR(255) UNIQUE NOT NULL,
    role VARCHAR(50) DEFAULT 'Student',
    created_at TIMESTAMP WITH TIME ZONE DEFAULT CURRENT_TIMESTAMP
);

-- 2. Exam Attempts & Scores Table
CREATE TABLE IF NOT EXISTS exam_attempts (
    attempt_id UUID PRIMARY KEY DEFAULT gen_random_uuid(),
    user_id UUID REFERENCES sovereign_users(user_id) ON DELETE CASCADE,
    week_number INT NOT NULL CHECK (week_number >= 0 AND week_number <= 10),
    score_achieved INT NOT NULL,
    total_possible INT NOT NULL,
    passed BOOLEAN NOT NULL,
    execution_time_ms INT NOT NULL,
    timestamp TIMESTAMP WITH TIME ZONE DEFAULT CURRENT_TIMESTAMP
);

-- 3. Interactive Simulator Highscores Table
CREATE TABLE IF NOT EXISTS simulator_highscores (
    score_id UUID PRIMARY KEY DEFAULT gen_random_uuid(),
    user_id UUID REFERENCES sovereign_users(user_id) ON DELETE CASCADE,
    perfect_score BOOLEAN DEFAULT FALSE,
    answers_breakdown JSONB NOT NULL,
    timestamp TIMESTAMP WITH TIME ZONE DEFAULT CURRENT_TIMESTAMP
);

-- 4. SHA-256 Biometric Integrity Logs Table
CREATE TABLE IF NOT EXISTS sha256_biometric_logs (
    log_id UUID PRIMARY KEY DEFAULT gen_random_uuid(),
    verified_hash VARCHAR(64) NOT NULL,
    dom_intact BOOLEAN NOT NULL,
    client_ip VARCHAR(45) NOT NULL,
    user_agent TEXT NOT NULL,
    timestamp TIMESTAMP WITH TIME ZONE DEFAULT CURRENT_TIMESTAMP
);

-- 5. Prompt Engineering Audit Queries Table
CREATE TABLE IF NOT EXISTS prompt_engineering_audit (
    query_id UUID PRIMARY KEY DEFAULT gen_random_uuid(),
    prompt_used TEXT NOT NULL,
    defense_triggered BOOLEAN DEFAULT FALSE,
    injection_detected BOOLEAN DEFAULT FALSE,
    response_tokens INT NOT NULL,
    timestamp TIMESTAMP WITH TIME ZONE DEFAULT CURRENT_TIMESTAMP
);

-- Indexes for lightning-fast database lookups
CREATE INDEX IF NOT EXISTS idx_exam_attempts_user ON exam_attempts(user_id);
CREATE INDEX IF NOT EXISTS idx_biometric_logs_time ON sha256_biometric_logs(timestamp DESC);
CREATE INDEX IF NOT EXISTS idx_prompt_audit_injection ON prompt_engineering_audit(injection_detected);
