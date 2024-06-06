<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">

    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Document</title>
</head>

<body>
    <!-- PARTIE PHP -->
    <?php
    if(isset($_POST) && !empty($_POST)){
        ?> <pre> <?php print_r($_POST); ?> </pre> <?php
        ?> <pre> <?php print_r($_FILES); ?> </pre> <?php

        if($_FILES['fichier']['error'] == 0){
            //Upload le fichier dans le dossier uploads
            move_uploaded_file($_FILES['fichier']['tmp_name'], 'uploads/' . $_FILES['fichier']['name']);
            echo 'Fichier envoyé avec succès';
            if($_FILES['fichier']['size']> 1500000){
                $error = 'Le fichier est trop lourd';
            }
        }else{
            $error = 'Erreur lors de l\'envoi du fichier';
        }
        // 1.5 Mo maximum pour le poids du fichier
        

    }



    



    ?>

    <!-- FORMULAIRE HTML -->

    <div style="color:red">
        <p>
            <?php if(isset($error)) echo $error; ?>
        </p>
    </div>
    <form method="POST" action="#" enctype="multipart/form-data">

        <input type="file" name="fichier" value="">
        <input type="submit" value="Envoyer" value="Charger le fichier">
    </form>


</body>

</html>