module Extension exposing (..)

import Html exposing (Html, button, div, h5, i, text)
import Html.Attributes exposing (class, type_)
import Html.Events exposing (onClick)
import Types exposing (Extension, Msg(..))


toHtml_ : Extension -> Html Msg
toHtml_ extension =
    div
        [ class "card mt-3" ]
        [ div
            [ class "card-body" ]
            [ div
                [ class "row justify-content-between" ]
                [ div
                    [ class "col-md-12" ]
                    [ h5
                        [ class "card-title" ]
                        [ text extension.name ]
                    ]
                , div [ class "col-md-12" ]
                    [ button
                        [ type_ "button", class "btn btn-outline-success btn-sm", onClick (StartExtension extension.name) ]
                        [ i [ class "fas fa-play" ] [], text " Play" ]
                    ]
                ]
            ]
        ]


toHtml : List Extension -> List (Html Msg)
toHtml extensions =
    List.map toHtml_ extensions


fromString : List String -> List Extension
fromString names =
    List.map (\name -> Extension name) names
