function updateData() {
    fetch("/data.json")
        .then(response => response.json())
        .then(data => {
            document.getElementById("temp").textContent = data.temp;
            document.getElementById("hum").textContent = data.hum;
            document.getElementById("date").textContent = data.date;
            document.getElementById("time").textContent = data.time;
        })
        .catch(err => {
            console.warn("Veri alınamadı:", err);
        });
}

setInterval(updateData, 5000);
window.onload = updateData;