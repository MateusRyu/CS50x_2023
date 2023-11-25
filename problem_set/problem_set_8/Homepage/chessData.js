async function getChessData(chess) {
    let response = await fetch(chess.profile);
    let chessData = await response.json();
    chess.pic.src = await chessData.avatar;

    // Convert epoch time in second to milisecons
    let joined = await new Date(chessData.joined *  1000).toISOString().split('T')[0];

    chess.joined.innerHTML = joined;
    chess.username.innerHTML = await chessData.username
    chess.username.href = await chessData.url

    response = await fetch(chess.stats);
    let chessStats = await response.json();
    let rapid = await chessStats.chess_rapid;
    chess.rating.innerHTML = await rapid.last.rating;
    chess.win.innerHTML = await rapid.record.win;
    chess.draw.innerHTML = await rapid.record.draw;
    chess.loss.innerHTML = await rapid.record.loss;
    let bestTime = await new Date(chessData.joined *  1000).toISOString().split('T')[0];
    chess.best.innerHTML = await `${rapid.best.rating}, ${bestTime}`;
    chess.best.href = await rapid.best.game;
}

// Wait until DOM is loaded
window.addEventListener('DOMContentLoaded', function(event) {
    let playerId = "kanna_yukari";
    let chess = {
        pic: document.querySelector("#chessPic"),
        joined: document.querySelector("#chessJoined"),
        username: document.querySelector("#chessUsername"),
        rating: document.querySelector("#chessRating"),
        win: document.querySelector("#chessWin"),
        draw: document.querySelector("#chessDraw"),
        loss: document.querySelector("#chessLoss"),
        best: document.querySelector("#chessBest"),
        profile: `https://api.chess.com/pub/player/${playerId}`,
        stats: `https://api.chess.com/pub/player/${playerId}/stats`
    };

    getChessData(chess);
});
