/**
 * 🏛️ AEL Sovereign Supabase & Cloud Persistence SDK
 * ==================================================
 * Enterprise JavaScript Connector designed by Ayman Elmasry.
 * Manages real-time data persistence, exam scoring logs, biometric SHA-256 
 * integrity tracking, and dynamic synchronization with Supabase PostgreSQL.
 */

class AELSovereignCloudConnector {
  constructor(options = {}) {
    // Inject Supabase credentials from environment or fallback to production presets
    this.supabaseUrl = options.supabaseUrl || 'https://YOUR_SUPABASE_PROJECT.supabase.co';
    this.supabaseKey = options.supabaseKey || 'eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9...YOUR_ANON_KEY';
    this.author = 'Ayman Elmasry';
    this.seal = 'AEL Sovereign Architecture';
    this.defaultUsername = options.username || 'aymanelmasryael';
    
    // Check if official Supabase SDK library is loaded globally, otherwise use native REST Fetch
    this.isNativeClient = typeof window !== 'undefined' && window.supabase;
    if (this.isNativeClient) {
      this.client = window.supabase.createClient(this.supabaseUrl, this.supabaseKey);
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
      throw error;
    }
  }

  /**
   * 💾 Log Exam Submissions & Code Execution Results to PostgreSQL
   * @param {Object} submissionData 
   */
  async logExamSubmission({ slug, sourceCode, language, score, passedChecks, executionTimeMs }) {
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

    console.log(`[AEL Cloud Connector] Pushing submission payload for slug '${slug}' to Supabase...`);

    if (this.isNativeClient) {
      const { data, error } = await this.client.from('exam_submissions').insert([payload]);
      if (error) throw error;
      return data;
    } else {
      return await this._request('exam_submissions', 'POST', payload);
    }
  }

  /**
   * 🛡️ Record Cryptographic DOM SHA-256 Integrity Verification Logs
   * @param {Object} integrityData 
   */
  async logBiometricIntegrity({ domHash, isTampered, userAgent, violationDetails }) {
    const payload = {
      dom_hash: domHash,
      is_tampered: isTampered || false,
      client_agent: userAgent || navigator.userAgent,
      violation_details: violationDetails || 'None. Flawless DOM validation.',
      inspected_by: this.author
    };

    console.log(`[AEL Cloud Connector] Registering DOM Integrity Hash (${domHash.substring(0, 12)}...)`);

    if (this.isNativeClient) {
      const { data, error } = await this.client.from('biometric_integrity').insert([payload]);
      if (error) throw error;
      return data;
    } else {
      return await this._request('biometric_integrity', 'POST', payload);
    }
  }

  /**
   * 📊 Fetch Real-Time Analytics & Grade Summaries for User
   */
  async fetchUserGradeSummary(username = this.defaultUsername) {
    if (this.isNativeClient) {
      const { data, error } = await this.client
        .from('exam_submissions')
        .select('*')
        .eq('username', username)
        .order('created_at', { ascending: false });
      if (error) throw error;
      return data;
    } else {
      return await this._request('exam_submissions', 'GET', null, `?username=eq.${username}&order=created_at.desc`);
    }
  }

  /**
   * ⚡ Real-Time WebSocket Channel Listener for Active Evaluation Updates
   */
  subscribeToSubmissionUpdates(callback) {
    if (!this.isNativeClient) {
      console.warn('🚨 Real-time WebSocket subscriptions require the official Supabase JS SDK.');
      return;
    }

    console.log('[AEL Cloud Connector] Opening WebSocket subscription on table: exam_submissions');
    return this.client
      .channel('custom-all-channel')
      .on('postgres_changes', { event: '*', schema: 'public', table: 'exam_submissions' }, payload => {
        console.log('⚡ [AEL Real-Time Stream] Database modification detected:', payload);
        if (typeof callback === 'function') callback(payload);
      })
      .subscribe();
  }
}

// Export connector globally for immediate integration inside the SPA Citadel
if (typeof window !== 'undefined') {
  window.AELSovereignCloudConnector = AELSovereignCloudConnector;
}
