# GitHub'a Upload Etme Rehberi

Bu rehber, RecoverySoftNetz dokümanlarını GitHub repository'nize yüklemeniz için gerekli adımları içermektedir.

## 📋 Hazırlanan Dosyalar

✅ README.md (GitHub ana sayfa)
✅ LICENSE (Proprietary license)
✅ .gitignore (Git ignore kuralları)
✅ RecoverySoftNetz_Technical_Documentation_TR.md (Ana teknik dokümantasyon)
✅ RecoverySoftNetz_Executive_Summary_EN.md (Yönetici özeti)
✅ RecoverySoftNetz_Implementation_Roadmap.md (Implementasyon planı)

---

## 🚀 Yöntem 1: Komut Satırı ile Upload (Önerilen)

### Adım 1: GitHub Repository'yi Clone Edin

Terminal veya Command Prompt'u açın ve aşağıdaki komutları çalıştırın:

```bash
# Repository'yi clone edin
git clone https://github.com/lekesiz/RSN.git

# Klasöre girin
cd RSN
```

### Adım 2: Dosyaları Kopyalayın

Claude'un hazırladığı dosyaları indirin ve RSN klasörüne kopyalayın:

```bash
# Eğer dosyalar /mnt/user-data/outputs'ta ise
# (Bu komutu Claude'un çalıştığı ortamda kullanın)
cp /mnt/user-data/outputs/*.md .
cp /mnt/user-data/outputs/LICENSE .
cp /mnt/user-data/outputs/.gitignore .
```

### Adım 3: Git'e Ekleyin ve Commit Edin

```bash
# Tüm dosyaları staging area'ya ekleyin
git add .

# Commit mesajı ile kaydedin
git commit -m "docs: Add comprehensive RecoverySoftNetz documentation

- Add technical documentation (TR)
- Add executive summary (EN)
- Add implementation roadmap
- Add README, LICENSE, and .gitignore"

# GitHub'a push edin
git push origin main
```

---

## 🌐 Yöntem 2: GitHub Web Interface ile Upload

### Adım 1: GitHub'a Giriş Yapın

1. https://github.com/lekesiz/RSN adresine gidin
2. GitHub hesabınızla giriş yapın

### Adım 2: Dosyaları Tek Tek Upload Edin

1. Repository ana sayfasında **"Add file"** > **"Upload files"** seçeneğini tıklayın
2. Aşağıdaki dosyaları sürükle-bırak ile veya "choose your files" ile seçin:
   - README.md
   - LICENSE
   - .gitignore
   - RecoverySoftNetz_Technical_Documentation_TR.md
   - RecoverySoftNetz_Executive_Summary_EN.md
   - RecoverySoftNetz_Implementation_Roadmap.md

3. Commit mesajı yazın:
   ```
   docs: Add comprehensive RecoverySoftNetz documentation
   ```

4. **"Commit changes"** butonuna tıklayın

---

## 📁 Yöntem 3: GitHub Desktop ile Upload

### Adım 1: GitHub Desktop'u İndirin

https://desktop.github.com/ adresinden GitHub Desktop'u indirin ve yükleyin

### Adım 2: Repository'yi Clone Edin

1. GitHub Desktop'u açın
2. **File > Clone Repository** seçin
3. `lekesiz/RSN` yazın veya listeden seçin
4. Yerel bir klasör seçin ve **Clone** butonuna tıklayın

### Adım 3: Dosyaları Ekleyin

1. Dosya gezgini ile clone ettiğiniz klasörü açın
2. Claude'un hazırladığı dosyaları bu klasöre kopyalayın

### Adım 4: Commit ve Push

1. GitHub Desktop otomatik olarak değişiklikleri gösterecektir
2. Sol altta commit mesajı yazın:
   ```
   Add comprehensive RecoverySoftNetz documentation
   ```
3. **"Commit to main"** butonuna tıklayın
4. Üstte **"Push origin"** butonuna tıklayın

---

## ✅ Doğrulama

Upload işleminden sonra şunları kontrol edin:

1. **README.md görünüyor mu?**
   - https://github.com/lekesiz/RSN adresine gidin
   - Ana sayfa otomatik olarak README'yi göstermelidir

2. **Tüm dosyalar mevcut mu?**
   - Repository'de 6 dosya olmalı:
     - README.md
     - LICENSE
     - .gitignore
     - RecoverySoftNetz_Technical_Documentation_TR.md
     - RecoverySoftNetz_Executive_Summary_EN.md
     - RecoverySoftNetz_Implementation_Roadmap.md

3. **Linkler çalışıyor mu?**
   - README.md içindeki dosya linklerine tıklayın
   - Her link ilgili dökümana yönlendirmeli

---

## 🔧 Sorun Giderme

### Problem: "Authentication failed" hatası

**Çözüm:**
```bash
# Personal Access Token kullanın
# GitHub'da: Settings > Developer settings > Personal access tokens > Generate new token
# Token'ı kopyalayın ve şifre yerine kullanın
```

### Problem: "Permission denied" hatası

**Çözüm:**
```bash
# SSH key ekleyin
ssh-keygen -t ed25519 -C "your-email@example.com"
# GitHub'da: Settings > SSH and GPG keys > New SSH key
# Public key'i kopyalayıp GitHub'a ekleyin
```

### Problem: Merge conflict

**Çözüm:**
```bash
# Önce remote'daki değişiklikleri çekin
git pull origin main

# Conflict'leri çözün
# Sonra commit ve push edin
git add .
git commit -m "Resolve merge conflicts"
git push origin main
```

---

## 📝 İpuçları

1. **Commit Mesajları:** Anlamlı ve açıklayıcı commit mesajları kullanın
2. **Branching:** Büyük değişiklikler için yeni branch oluşturun
3. **Issues:** GitHub Issues kullanarak görevleri takip edin
4. **Wiki:** Ek dokümantasyon için GitHub Wiki'yi kullanabilirsiniz
5. **Releases:** Milestone'lar için GitHub Releases kullanın

---

## 🎯 Sonraki Adımlar

Upload tamamlandıktan sonra:

1. ✅ Repository'yi **public** veya **private** olarak ayarlayın
2. ✅ **About** kısmına proje açıklaması ekleyin
3. ✅ **Topics** ekleyin (data-recovery, ai, blockchain, etc.)
4. ✅ **GitHub Pages** aktif edin (Settings > Pages) - optional
5. ✅ **Branch protection** kuralları ekleyin (Settings > Branches)

---

## 📞 Yardım

Herhangi bir sorun yaşarsanız:
- GitHub Docs: https://docs.github.com
- GitHub Support: https://support.github.com

---

**Not:** Bu rehber RecoverySoftNetz projesi için özel olarak hazırlanmıştır.

Last Updated: October 31, 2025
