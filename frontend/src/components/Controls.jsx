export default function Controls({ 
  mode, 
  setMode, 
  onClear,
  onRemove,
  kValue,
  setKValue,
  radiusValue,
  setRadiusValue
}) {
  return (
    <>
      {/* Mode Selection */}
      <div className="glass-card">
        <div className="card-title">Mode</div>
        <div className="mode-buttons">
          <button
            id="btn-driver-mode"
            className={`mode-btn driver ${mode === 'driver' ? 'active' : ''}`}
            onClick={() => setMode('driver')}
          >
            <div className="mode-btn-icon">🚗</div>
            <div className="mode-btn-text">
              <span>Add Driver</span>
              <span>Click to place / drag to move</span>
            </div>
          </button>
          
          <button
            id="btn-passenger-mode"
            className={`mode-btn passenger ${mode === 'passenger' ? 'active' : ''}`}
            onClick={() => setMode('passenger')}
          >
            <div className="mode-btn-icon">🧑</div>
            <div className="mode-btn-text">
              <span>Nearest Neighbor</span>
              <span>Find the single closest driver</span>
            </div>
          </button>

          <button
            id="btn-find-k-mode"
            className={`mode-btn find-k ${mode === 'find-k' ? 'active' : ''}`}
            onClick={() => setMode('find-k')}
          >
            <div className="mode-btn-icon">📶</div>
            <div className="mode-btn-text">
              <span>K-Nearest</span>
              <span>Find the K closest drivers</span>
            </div>
          </button>

          <button
            id="btn-range-mode"
            className={`mode-btn range ${mode === 'range' ? 'active' : ''}`}
            onClick={() => setMode('range')}
          >
            <div className="mode-btn-icon">⭕</div>
            <div className="mode-btn-text">
              <span>Range Search</span>
              <span>Find drivers within radius</span>
            </div>
          </button>
        </div>
      </div>

      {/* Parameters */}
      {(mode === 'find-k' || mode === 'range') && (
        <div className="glass-card">
          <div className="card-title">Parameters</div>
          <div className="param-controls">
            {mode === 'find-k' && (
              <div className="param-item">
                <label>K Value (1-10): {kValue}</label>
                <input 
                  type="range" 
                  min="1" 
                  max="10" 
                  value={kValue} 
                  onChange={(e) => setKValue(parseInt(e.target.value))}
                />
              </div>
            )}
            {mode === 'range' && (
              <div className="param-item">
                <label>Radius (50-300): {radiusValue}</label>
                <input 
                  type="range" 
                  min="50" 
                  max="300" 
                  value={radiusValue} 
                  onChange={(e) => setRadiusValue(parseInt(e.target.value))}
                />
              </div>
            )}
          </div>
        </div>
      )}

      {/* Manual Management */}
      <div className="glass-card">
        <div className="card-title">Manual Management</div>
        <div className="manual-remove">
          <input 
            type="number" 
            placeholder="Driver ID" 
            id="manual-remove-input"
            className="id-input"
          />
          <button 
            className="action-btn danger"
            onClick={() => {
              const input = document.getElementById('manual-remove-input');
              const id = parseInt(input.value);
              if (!isNaN(id)) {
                onRemove(id);
                input.value = '';
              }
            }}
          >
            Delete ID
          </button>
        </div>
      </div>

      {/* Actions */}
      <div className="glass-card">
        <div className="card-title">General Actions</div>
        <div className="action-buttons">
          <button
            id="btn-clear-all"
            className="action-btn danger"
            onClick={onClear}
          >
            🗑️ Clear All
          </button>
        </div>
      </div>
    </>
  );
}
