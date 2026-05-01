const API_BASE = 'http://localhost:3001/api';

export async function listDrivers() {
  const res = await fetch(`${API_BASE}/drivers`);
  return res.json();
}

export async function addDriver(x, y) {
  const res = await fetch(`${API_BASE}/drivers`, {
    method: 'POST',
    headers: { 'Content-Type': 'application/json' },
    body: JSON.stringify({ x, y }),
  });
  return res.json();
}


export async function removeDriver(index) {
  const res = await fetch(`${API_BASE}/drivers/${index}`, {
    method: 'DELETE',
  });
  return res.json();
}

export async function findNearest(x, y) {
  const res = await fetch(`${API_BASE}/find`, {
    method: 'POST',
    headers: { 'Content-Type': 'application/json' },
    body: JSON.stringify({ x, y }),
  });
  return res.json();
}

export async function findKNearest(x, y, k) {
  const res = await fetch(`${API_BASE}/find-k`, {
    method: 'POST',
    headers: { 'Content-Type': 'application/json' },
    body: JSON.stringify({ x, y, k }),
  });
  return res.json();
}

export async function findRange(x, y, radius) {
  const res = await fetch(`${API_BASE}/range`, {
    method: 'POST',
    headers: { 'Content-Type': 'application/json' },
    body: JSON.stringify({ x, y, radius }),
  });
  return res.json();
}

export async function clearAll() {
  const res = await fetch(`${API_BASE}/clear`, {
    method: 'POST',
  });
  return res.json();
}

export async function healthCheck() {
  try {
    const res = await fetch(`${API_BASE}/health`);
    const data = await res.json();
    return data.cProcessReady;
  } catch {
    return false;
  }
}
