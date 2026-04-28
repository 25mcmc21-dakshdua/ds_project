export default function Stats({ drivers, matchResult, onRemoveDriver }) {
  const results = Array.isArray(matchResult) ? matchResult : (matchResult ? [matchResult] : []);
  const matchedCount = results.length;

  return (
    <>
      {/* Statistics */}
      <div className="glass-card">
        <div className="card-title">Statistics</div>
        <div className="stats-grid">
          <div className="stat-item">
            <div className="stat-value green">{drivers.length}</div>
            <div className="stat-label">Drivers</div>
          </div>
          <div className="stat-item">
            <div className="stat-value orange">{matchedCount}</div>
            <div className="stat-label">Results</div>
          </div>
          <div className="stat-item">
            <div className="stat-value blue">
              {results.length > 0 && results[0].distance !== undefined 
                ? results[0].distance.toFixed(1) 
                : '—'}
            </div>
            <div className="stat-label">Min Dist</div>
          </div>
          <div className="stat-item">
            <div className="stat-value purple">
              {results.length > 0 ? `#${results[0].index}` : '—'}
            </div>
            <div className="stat-label">Primary</div>
          </div>
        </div>
      </div>

      {/* Match Result */}
      <div className="glass-card match-result">
        <div className="card-title">Match Results ({matchedCount})</div>
        <div className="match-scroll">
          {results.length > 0 ? (
            results.map((res) => (
              <div key={res.index} className="match-card">
                <div className="match-header">
                  <span>🎯</span> Driver #{res.index}
                </div>
                <div className="match-details">
                  <div className="match-detail">
                    <span className="match-detail-label">Location</span>
                    <span className="match-detail-value">({res.x}, {res.y})</span>
                  </div>
                  {res.distance !== undefined && (
                    <div className="match-detail">
                      <span className="match-detail-label">Distance</span>
                      <span className="match-detail-value">{res.distance.toFixed(2)}</span>
                    </div>
                  )}
                </div>
              </div>
            ))
          ) : (
            <div className="no-match">No results found</div>
          )}
        </div>
      </div>

      {/* Driver List */}
      <div className="glass-card driver-list">
        <div className="card-title">All Drivers ({drivers.length})</div>
        {drivers.length > 0 ? (
          <div className="driver-scroll">
            {drivers.map((driver) => {
              const isMatched = results.some(res => res.index === driver.index);
              return (
                <div
                  key={driver.index}
                  className={`driver-item ${isMatched ? 'matched' : ''}`}
                >
                  <div className="driver-info">
                    <div className="driver-dot"></div>
                    <span className="driver-coords">
                      ({driver.x}, {driver.y})
                    </span>
                    <span className="driver-idx">#{driver.index}</span>
                  </div>
                  <button
                    className="driver-remove"
                    onClick={() => onRemoveDriver(driver.index)}
                    title="Remove driver"
                  >
                    ✕
                  </button>
                </div>
              );
            })}
          </div>
        ) : (
          <div className="empty-list">No drivers added yet</div>
        )}
      </div>
    </>
  );
}
