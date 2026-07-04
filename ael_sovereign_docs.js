    function switchMainTab(tabId) {
        document.querySelectorAll('.main-tab-view').forEach(view => view.classList.remove('active'));
        document.querySelectorAll('.switch-btn').forEach(btn => btn.classList.remove('active'));
        
        document.getElementById(`tab-${tabId}`).classList.add('active');
        event.currentTarget.classList.add('active');
        window.scrollTo({ top: 400, behavior: 'smooth' });
    }

    function copyRawFormat(contentId, statusElemId, successMsg) {
        const contentElem = document.getElementById(contentId);
        if (!contentElem) return;
        
        const textToCopy = contentElem.innerText || contentElem.textContent;
        navigator.clipboard.writeText(textToCopy.trim()).then(() => {
            const statusElem = document.getElementById(statusElemId);
            if (statusElem) {
                statusElem.style.display = 'flex';
                statusElem.innerHTML = `<i class="fas fa-check-circle"></i> <span>${successMsg}</span>`;
                setTimeout(() => {
                    statusElem.style.display = 'none';
                }, 4000);
            }
        }).catch(err => {
            console.error('Copy failed:', err);
        });
    }

    function downloadRawFormat(contentId, filename, mimeType, statusElemId, successMsg) {
        const contentElem = document.getElementById(contentId);
        if (!contentElem) return;
        
        const textContent = contentElem.innerText || contentElem.textContent;
        const blob = new Blob([textContent.trim()], { type: mimeType });
        const url = URL.createObjectURL(blob);
        const a = document.createElement('a');
        a.href = url;
        a.download = filename;
        document.body.appendChild(a);
        a.click();
        document.body.removeChild(a);
        URL.revokeObjectURL(url);

        const statusElem = document.getElementById(statusElemId);
        if (statusElem) {
            statusElem.style.display = 'flex';
            statusElem.innerHTML = `<i class="fas fa-file-export"></i> <span>${successMsg}</span>`;
            setTimeout(() => {
                statusElem.style.display = 'none';
            }, 4000);
        }
    }

    // نظام التحقق السيادي الحيوي
    async function sha256(message) {
        const msgBuffer = new TextEncoder().encode(message);
        const hashBuffer = await crypto.subtle.digest('SHA-256', msgBuffer);
        const hashArray = Array.from(new Uint8Array(hashBuffer));
        return hashArray.map(b => b.toString(16).padStart(2, '0')).join('');
    }

    async function verifySystemIntegrity() {
        const content = document.documentElement.outerHTML;
        const currentHash = await sha256(content);
        const baseline = localStorage.getItem('AEL_DOCS_BASELINE');
        const badge = document.querySelector('.live-badge') || document.querySelector('.directive-badge');
        
        if (!badge) return;

        if (!baseline) {
            localStorage.setItem('AEL_DOCS_BASELINE', currentHash);
            badge.innerHTML = '<i class="fas fa-circle"></i> SYSTEM SEALED & VERIFIED';
            badge.style.borderColor = '#00E699';
            badge.style.color = '#00E699';
        } else if (currentHash === baseline) {
            badge.innerHTML = '<i class="fas fa-circle"></i> SYSTEM INTEGRITY INTACT';
            badge.style.borderColor = '#00E699';
            badge.style.color = '#00E699';
        } else {
            badge.innerHTML = '<i class="fas fa-triangle-exclamation"></i> ⚠️ SYSTEM TAMPERED';
            badge.style.borderColor = '#FF4444';
            badge.style.color = '#FF4444';
        }
    }

    // تنفيذ التحقق عند تحميل الصفحة
    document.addEventListener('DOMContentLoaded', verifySystemIntegrity);
</script>
