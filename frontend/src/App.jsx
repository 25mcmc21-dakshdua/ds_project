import './index.css';
import Canvas from './components/Canvas';
import Controls from './components/Controls';
import Stats from './components/Stats';
import { useRideMatch } from './hooks/useRideMatch';

function App() {
  const {
    drivers,
    mode,
    setMode,
    matchResult,
    passengerPos,
    loading,
    connected,
    kValue,
    setKValue,
    radiusValue,
    setRadiusValue,
    handleCanvasClick,
    handleUpdateDriver,
    handleRemoveDriver,
    handleClear,
  } = useRideMatch();

  return (
    <div className="app">
      {/* Sidebar */}
      <aside className="sidebar">
        {/* Logo */}
        <div className="logo">
          <div className="logo-icon">🚕</div>
          <div>
            <h1>RideMatch</h1>
            <p>KD-Tree Spatial Search</p>
          </div>
        </div>

        {/* Controls */}
        <Controls 
          mode={mode} 
          setMode={setMode} 
          onClear={handleClear} 
          onRemove={handleRemoveDriver}
          kValue={kValue}
          setKValue={setKValue}
          radiusValue={radiusValue}
          setRadiusValue={setRadiusValue}
        />

        {/* Stats */}
        <Stats
          drivers={drivers}
          matchResult={matchResult}
          onRemoveDriver={handleRemoveDriver}
        />

        {/* Connection Status */}
        <div className="connection-status">
          <span className={`connection-dot ${connected ? 'connected' : 'disconnected'}`}></span>
          {connected ? 'C Process Connected' : 'Connecting...'}
        </div>
      </aside>

      {/* Canvas Area */}
      <main className="main-area">
        <Canvas
          drivers={drivers}
          matchResult={matchResult}
          passengerPos={passengerPos}
          mode={mode}
          loading={loading}
          radius={radiusValue}
          onCanvasClick={handleCanvasClick}
          onUpdateDriver={handleUpdateDriver}
        />
      </main>
    </div>
  );
}

export default App;
