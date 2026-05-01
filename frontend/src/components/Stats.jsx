export default function Stats({ drivers, matchResult, onRemoveDriver }) {
  const results = Array.isArray(matchResult) ? matchResult : (matchResult ? [matchResult] : []);
  const matchedCount = results.length;

  return (
    <>
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

    </>
  );
}
