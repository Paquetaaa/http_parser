<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Document</title>
</head>

<body>
    <!-- PARTIE HTML -->
    <form action="#" method="post" enctype="multipart/form-data">
        <input type="file" name="fichier" value="">
        <input type="submit" name="chargement" value="Envoyer le fichier">
    </form>

    <!-- PARTIE PHP -->
    <?php
    if(isset($_POST) && !empty($_POST)){
	echo '<pre>'; print_r($_POST); echo '</pre>';
        echo '<pre>'; print_r($_FILES); echo '</pre>';

        if($_FILES['fichier']['error'] == 0){
            // 1.5 Mo maximum pour le poids du fichier
            if($_FILES['fichier']['size']> 1500000){
                $error = 'Le fichier est trop lourd';
            }

            // Vérifier le type de fichier
            $extensions = strrchr($_FILES['fichier']['name'], '.');
            if($extensions != '.jpg'){
                $error = 'Le fichier n\'est pas une image jpg';
            }

            if(!isset($error)){
                //Upload le fichier dans le dossier upload
                if (!is_dir('upload')) {
                    echo 'Le dossier "upload" n\'existe pas.';
                } else if (!is_writable('upload')) {
                    echo 'Le serveur n\'a pas les permissions pour écrire dans le dossier "upload".';
                }
                move_uploaded_file($_FILES['fichier']['tmp_name'], 'upload/' . $_FILES['fichier']['name']);
                echo 'Fichier envoyé avec succès';
            }else{
                echo $error;
            }
        }else{
            echo 'Erreur lors de l\'envoi du fichier';
        }
    }
    ?>
</body>
</html>
