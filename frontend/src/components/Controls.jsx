export default function Controls({ 
  mode, 
  setMode, 
  onClear,
  radiusValue,
  setRadiusValue,
  onFindNearest,
  onSearchRadius
}) {
  return (
    <>
      {/* Mode Selection */}
      <div className="glass-card">
        <div className="card-title">Actions</div>
        <div className="mode-buttons">
          <button
            id="btn-add-driver"
            className={`mode-btn driver ${mode === 'add-driver' ? 'active' : ''}`}
            onClick={() => setMode('add-driver')}
          >
            <div className="mode-btn-icon">🚗</div>
            <div className="mode-btn-text">
              <span>Add Driver</span>
              <span>Click to place driver</span>
            </div>
          </button>
          
          <button
            id="btn-add-passenger"
            className={`mode-btn passenger ${mode === 'add-passenger' ? 'active' : ''}`}
            onClick={() => setMode('add-passenger')}
          >
            <div className="mode-btn-icon">🧑</div>
            <div className="mode-btn-text">
              <span>Add Passenger</span>
              <span>Place/Move passenger</span>
            </div>
          </button>

          <button
            id="btn-find-nearest"
            className={`mode-btn nearest ${mode === 'find-nearest' ? 'active' : ''}`}
            onClick={() => {
              setMode('find-nearest');
              onFindNearest();
            }}
          >
            <div className="mode-btn-icon">🎯</div>
            <div className="mode-btn-text">
              <span>Find Nearest</span>
              <span>Locate closest driver</span>
            </div>
          </button>

          <button
            id="btn-search-radius"
            className={`mode-btn range ${mode === 'search-radius' ? 'active' : ''}`}
            onClick={() => {
              setMode('search-radius');
              onSearchRadius();
            }}
          >
            <div className="mode-btn-icon">⭕</div>
            <div className="mode-btn-text">
              <span>Search in Radius</span>
              <span>Find within distance</span>
            </div>
          </button>

          <button
            id="btn-delete-driver"
            className={`mode-btn delete ${mode === 'delete-driver' ? 'active' : ''}`}
            onClick={() => setMode('delete-driver')}
          >
            <div className="mode-btn-icon">❌</div>
            <div className="mode-btn-text">
              <span>Delete Driver</span>
              <span>Click driver to remove</span>
            </div>
          </button>
        </div>
      </div>

      {/* Parameters */}
      {mode === 'search-radius' && (
        <div className="glass-card">
          <div className="card-title">Search Parameters</div>
          <div className="param-controls">
            <div className="param-item">
              <label>Radius (50-300): {radiusValue}</label>
              <input 
                type="range" 
                min="50" 
                max="300" 
                value={radiusValue} 
                onChange={(e) => {
                  setRadiusValue(parseInt(e.target.value));
                  // Optionally trigger search again if radius changes
                }}
              />
            </div>
          </div>
        </div>
      )}

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
