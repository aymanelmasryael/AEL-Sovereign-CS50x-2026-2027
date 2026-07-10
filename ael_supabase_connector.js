/**
 * 🏛️ AEL Sovereign Supabase & Cloud Persistence SDK
 * ==================================================
 * Enterprise JavaScript Connector designed by Ayman Elmasry.
 * Manages real-time data persistence, exam scoring logs, biometric SHA-256 
 * integrity tracking, and dynamic synchronization with Supabase PostgreSQL.
 */

class AELSovereignCloudConnector {
  constructor(options = {}) {
    // Load Supabase credentials: options > env vars > localStorage > graceful fallback
    const envUrl = typeof SUPABASE_URL !== 'undefined' ? SUPABASE_URL : null;
    const envKey = typeof SUPABASE_ANON_KEY !== 'undefined' ? SUPABASE_ANON_KEY : null;
    this.supabaseUrl = options.supabaseUrl || envUrl || localStorage.getItem('ael_supabase_url') || null;
    this.supabaseKey = options.supabaseKey || envKey || localStorage.getItem('ael_supabase_key') || null;
    this.author = 'Ayman Elmasry';
    this.seal = 'AEL Sovereign Architecture';
    this.defaultUsername = options.username || 'aymanelmasryael';
    
    // Track whether real Supabase credentials are configured
    this.isConfigured = !!(this.supabaseUrl && this.supabaseKey && 
      this.supabaseUrl !== 'null' && this.supabaseKey !== 'null');
    
    if (!this.isConfigured) {
      console.info('[AEL Cloud Connector] Supabase not configured — running in offline mode. Set SUPABASE_URL and SUPABASE_ANON_KEY env vars to enable cloud persistence.');
    }
    
    if (this.isConfigured && typeof window !== 'undefined' && window.supabase) {
      this.isNativeClient = true;
      this.client = window.supabase.createClient(this.supabaseUrl, this.supabaseKey);
    } else {
      this.isNativeClient = false;
    }
  }

  /**
   * Helper utility for executing native REST requests to PostgREST API
   */
  async _request(table, method = 'POST', payload = null, queryParams = '') {
    const url = `${this.supabaseUrl}/rest/v1/${table}${queryParams}`;
    const headers = {
      'apikey': this.supabaseKey,
      'Authorization': `Bearer ${this.supabaseKey}`,
      'Content-Type': 'application/json',
      'Prefer': 'return=representation'
    };

    if (!this.isConfigured) {
      console.info(`[AEL Cloud Connector] Offline mode — skipping ${method} ${table}`);
      return { offline: true, message: 'Supabase not configured' };
    }
    try {
      const response = await fetch(url, {
        method,
        headers,
        body: payload ? JSON.stringify(payload) : null
      });

      if (!response.ok) {
        const errorText = await response.text();
        throw new Error(`Cloud Persistence Error (${response.status}): ${errorText}`);
      }

      return await response.json();
    } catch (error) {
      console.error(`🚨 [AEL Cloud Connector] API Failure on table '${table}':`, error.message);
      return { offline: true, error: error.message };
    }
  }

  /**
   * 💾 Log Exam Submissions & Code Execution Results to PostgreSQL
   * @param {Object} submissionData 
   */
  async logExamSubmission({ slug, sourceCode, language, score, passedChecks, executionTimeMs }) {
    if (!this.isConfigured) return { offline: true, message: 'Cloud persistence not configured' };
    const payload = {
      username: this.defaultUsername,
      slug: slug || 'cs50/problems/2026/x/general',
      source_code: sourceCode,
      language: language || 'c',
      score: score || 0,
      passed_checks: passedChecks || false,
      execution_time_ms: executionTimeMs || 0.0,
      biometric_seal: this.seal
    };
    if (this.isNativeClient) {
      const { data, error } = await this.client.from('exam_submissions').insert([payload]);
      return error ? { offline: true, error: error.message } : data;
    }
    return await this._request('exam_submissions', 'POST', payload);
  }

  /**
   * 🛡️ Record Cryptographic DOM SHA-256 Integrity Verification Logs
   */
  async logBiometricIntegrity({ domHash, isTampered, userAgent, violationDetails }) {
    if (!this.isConfigured) return { offline: true, message: 'Cloud persistence not configured' };
    const payload = {
      dom_hash: domHash,
      is_tampered: isTampered || false,
      client_agent: userAgent || navigator.userAgent,
      violation_details: violationDetails || 'None. Flawless DOM validation.',
      inspected_by: this.author
    };
    if (this.isNativeClient) {
      const { data, error } = await this.client.from('biometric_integrity').insert([payload]);
      return error ? { offline: true, error: error.message } : data;
    }
    return await this._request('biometric_integrity', 'POST', payload);
  }

  /**
   * 📊 Fetch Real-Time Analytics & Grade Summaries for User
   */
  async fetchUserGradeSummary(username = this.defaultUsername) {
    if (!this.isConfigured) return { offline: true, message: 'Cloud persistence not configured' };
    if (this.isNativeClient) {
      const { data, error } = await this.client
        .from('exam_submissions')
        .select('*')
        .eq('username', username)
        .order('created_at', { ascending: false });
      return error ? { offline: true, error: error.message } : data;
    }
    return await this._request('exam_submissions', 'GET', null, `?username=eq.${username}&order=created_at.desc`);
  }

  /**
   * ⚡ Real-Time WebSocket Channel Listener for Active Evaluation Updates
   */
  subscribeToSubmissionUpdates(callback) {
    if (!this.isNativeClient) {
      console.warn('🚨 Real-time WebSocket subscriptions require the official Supabase JS SDK.');
      return;
    }

    return this.client
      .channel('custom-all-channel')
      .on('postgres_changes', { event: '*', schema: 'public', table: 'exam_submissions' }, payload => {
        if (typeof callback === 'function') callback(payload);
      })
      .subscribe();
  }
}

// Export connector globally for immediate integration inside the SPA Citadel
if (typeof window !== 'undefined') {
  window.AELSovereignCloudConnector = AELSovereignCloudConnector;
}
