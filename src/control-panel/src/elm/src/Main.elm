port module Main exposing (..)

import Browser
import Html exposing (Html, button, div, h1, h5, i, text)
import Html.Attributes exposing (class, id, type_)
import Html.Events exposing (onClick)



-- PORTS


port startExtension : String -> Cmd msg


type alias Model =
    { extensions : List String }


init : List String -> ( Model, Cmd msg )
init extensions =
    ( { extensions = extensions }, Cmd.none )


subscriptions : a -> Sub msg
subscriptions _ =
    Sub.none


type Msg
    = StartExtension String


main : Program (List String) Model Msg
main =
    Browser.element { init = init, update = update, view = view, subscriptions = subscriptions }


update : Msg -> Model -> ( Model, Cmd Msg )
update msg model =
    case msg of
        StartExtension extension ->
            ( model, startExtension extension )


listView : String -> Html Msg
listView extension =
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
                        [ text extension ]
                    ]
                , div [ class "col-md-12" ]
                    [ button
                        [ type_ "button", class "btn btn-outline-success btn-sm", onClick (StartExtension extension) ]
                        [ i [ class "fas fa-play" ] [], text " Play" ]
                    ]
                ]
            ]
        ]


view : Model -> Html Msg
view model =
    div [ class "container mt-1" ]
        [ h1 [] [ text "Extensions" ]
        , div [ id "extensions-list" ] (List.map listView model.extensions)
        ]
