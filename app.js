let hitungWarisCplusPlus;
let teksHasilBuffer = "";

if (typeof Module === 'undefined') {
    Module = {};
}
Module.print = function(text) {
    console.log(text);
    teksHasilBuffer += text + "\n";
};

Module.onRuntimeInitialized = () => {
    hitungWarisCplusPlus = Module.cwrap('hitungWarisWasm', null, [
        'number', 'number', 'number', 'number', 'number', 'number', 'number', 'number', 'number'
    ]);
    console.log("WebAssembly C++ Berhasil Dimuat!");
};

// Only run calculator logic if form exists (calculator.html page)
const formWaris = document.getElementById('formWaris');
if (formWaris) {

    const jenisKelaminSelect = document.getElementById('jenisKelaminMayit');
    const labelPasangan = document.getElementById('labelPasangan');

    // Interaksi UX: Mengubah label pasangan sesuai jenis kelamin jenazah
    jenisKelaminSelect.addEventListener('change', (e) => {
        if (e.target.value === 'P') {
            labelPasangan.textContent = 'Apakah Suami Hidup?';
        } else {
            labelPasangan.textContent = 'Apakah Istri Hidup?';
        }
    });

    // Event Handler saat tombol Hitung diklik
    formWaris.addEventListener('submit', function(e) {
        e.preventDefault();

        if (!hitungWarisCplusPlus) {
            alert("Modul WebAssembly belum siap, harap tunggu sebentar.");
            return;
        }

        teksHasilBuffer = "";

        const hartaKotor = parseFloat(document.getElementById('hartaKotor').value) || 0;
        const hutang     = parseFloat(document.getElementById('hutang').value) || 0;
        const biayaMakam = parseFloat(document.getElementById('biayaMakam').value) || 0;
        const jenisKelaminVal = document.getElementById('jenisKelaminMayit').value;
        const jkChar = jenisKelaminVal.charCodeAt(0);
        const isPasangan = parseInt(document.getElementById('isSuamiIstriHidup').value) || 0;
        const isAyah     = parseInt(document.getElementById('isAyahHidup').value) || 0;
        const isIbu      = parseInt(document.getElementById('isIbuHidup').value) || 0;
        const anakLk     = parseInt(document.getElementById('jumlahAnakLk').value) || 0;
        const anakPr     = parseInt(document.getElementById('jumlahAnakPr').value) || 0;

        hitungWarisCplusPlus(hartaKotor, hutang, biayaMakam, jkChar, isPasangan, isAyah, isIbu, anakLk, anakPr);

        const elemenOutput = document.getElementById('output-wasm');
        const containerHasil = document.getElementById('section-hasil');
        const namaAlmarhum = document.getElementById('namaAlmarhum').value || '-';
        const hartaBersih = hartaKotor - (hutang + biayaMakam);

        elemenOutput.textContent = teksHasilBuffer;
        containerHasil.classList.remove('hidden');

        // Update ringkasan
        document.getElementById('resNamaAlmarhum').textContent = namaAlmarhum;
        document.getElementById('resHartaBersih').textContent = 'Rp ' + hartaBersih.toLocaleString('id-ID');

        containerHasil.scrollIntoView({ behavior: 'smooth' });
    });
}
